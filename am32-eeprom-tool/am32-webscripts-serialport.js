var serport = null;
var serport_reader = null;
var serport_writer = null;

const ack_byte = 0x30;

var serport_fifo = null;
var serport_echocnt = 0;
var serport_hasQuery = false;
var serport_timeout = 1000;
const serport_timeout_default = 2000;
const serport_startwait_default = 40;

async function serport_connect()
{
    var toopen = false;
    if (serport == null) {
        toopen = true;
    }
    if (toopen)
    {
        debug_textbox.value = "";
        try
        {
            const port = await navigator.serial.requestPort();

            await port.open({ baudRate: 19200 });

            serport = port;
            serport_writer = await port.writable.getWriter();
            serport_reader = await port.readable.getReader();

            var btn = document.getElementById("btn_serconnect");
            btn.value = "Disconnect";
            btn.onclick = serport_disconnect;
            document.getElementById("span_serport").innerHTML = "(connected)";
            serport_buttonsSetDisabled(false);
            serport_hasQuery = false;
            serport_fifo = null;

            port.addEventListener("connect", (event) => {
                console.log("port on connect");
                var btn = document.getElementById("btn_serconnect");
                btn.value = "Disconnect";
                btn.onclick = serport_disconnect;
                document.getElementById("span_serport").innerHTML = "(connected)";
                serport_buttonsSetDisabled(false);
                serport_hasQuery = false;
                serport_fifo = null;
            });

            port.addEventListener("disconnect", (event) => {
                console.log("port on disconnect");
                serport = null;
                var btn = document.getElementById("btn_serconnect");
                btn.value = "Connect";
                btn.onclick = serport_connect;
                document.getElementById("span_serport").innerHTML = "";
                serport_buttonsSetDisabled(true);
                serport_hasQuery = false;
                serport_fifo = null;
            });
        }
        catch (ex)
        {
            debug_textbox.value = "FAILED to open serial port, error exception: " + ex.toString();
        }
    }
}

async function serport_disconnect()
{
    if (serport != null)
    {
        try { serport_writer.releaseLock(); } catch { }
        try { serport_reader.releaseLock(); } catch { }
        serport.close();
    }
    var btn = document.getElementById("btn_serconnect");
    btn.value = "Connect";
    btn.onclick = serport_connect;
    serport = null;
    document.getElementById("span_serport").innerHTML = "";
    serport_buttonsSetDisabled(true);
    serport_hasQuery = false;
    serport_fifo = null;
}

async function serport_readThread()
{
    
}

function serport_fifoPush(x)
{
    serport_fifo = uint8ArrayMerge(serport_fifo, x);
}

function serport_fifoPopBlindCnt(cnt)
{
    if (serport_fifo.length > cnt) {
        serport_fifo = serport_fifo.slice(cnt);
    }
    else {
        serport_fifo = new Uint8Array([]);
    }
}

async function serport_resetReader()
{
    serport_fifo = null;
}

async function serport_tx(data, expect, cb, start_wait=serport_startwait_default, total_wait=serport_timeout_default)
{
    if (serport == null) {
        return;
    }
    var buffer = new ArrayBuffer(data.length);
    var buffer8 = new Uint8Array(buffer);
    for (var i = 0; i < data.length; i++) { buffer8[i] = data[i]; }
    console.log("serport sending");
    console.log(buffer8);
    await serport_writer.write(buffer8);
    serport_lastTime = Date.now();
    serport_timeout = total_wait;
    setTimeout(async function () {
        await serport_txWait(data, expect, cb);
    }, start_wait);
}

async function serport_txWait(data, expect, cb)
{
    if (serport_fifo != null)
    {
        if (serport_fifo.length >= data.length + expect)
        {
            var chunk = serport_fifo.slice(0, data.length + expect);
            serport_fifoPopBlindCnt(data.length + expect);
            console.log("serport rx'ed");
            console.log(chunk);
            for (var i = 0; i < data.length; i++) {
                if (data[i] != chunk[i]) {
                    console.log("error verifying echo");
                    console.log(data);
                    debug_textbox.value += "warning: echo content does not match sent data\r\n";
                    break;
                }
            }
            if (cb) {
                cb(chunk.slice(data.length));
            }
            return;
        }
    }

    var t = Date.now();
    if ((t - serport_lastTime) > serport_timeout) {
        if (serport_fifo != null && serport_fifo.length == expect)
        {
            console.log("serport_txWait timed out but has expected data");
            if (cb) {
                cb(serport_fifo);
            }
            return;
        }
        else
        {
            console.log("serport_txWait timed out");
            if (serport_fifo != null && serport_fifo.length > 0) {
                console.log("serport_fifo.length = " + serport_fifo.length);
            }
            await serport_resetReader();
            if (cb) {
                cb(null);
            }
            return;
        }
    }

    if (serport.readable)
    {
        try
        {
            let { value, done } = await Promise.race([
                serport_reader.read(),
                new Promise((_, reject) => setTimeout(reject, 5, new Error("serport_txWait read timeout")))
                ]);
            if (value != null)
            {
                if (value.length > 0)
                {
                    console.log("serport read a bit");
                    console.log(value);
                    serport_fifoPush(value);
                    serport_lastTime = Date.now();
                }
            }
        }
        catch (ex)
        {
            console.log(ex);
        }
    }

    setTimeout(async function () {
        await serport_txWait(data, expect, cb);
    }, 5);
}

async function serport_readEepromInner(proc_cb)
{
    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(eeprom_offset), 1
        , async function (data) { // sent set address

        if (data == false || data == null || data.length < 1) {
            debug_textbox.value += "ESC failed to ack address set command\r\n";
            serport_buttonsSetCurrent();
            return;
        }
        else if (data[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command (" + toHexString(data[0]) + ")\r\n";
            serport_buttonsSetCurrent();
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genReadCmd(eeprom_useful_length), eeprom_useful_length + 3
            , async function (data) { // sent read

            var data1 = data;
            if (data1 == null || data1 == false) {
                debug_textbox.value += "ESC failed to reply first data packet\r\n";
                serport_buttonsSetCurrent();
                return;
            }
            else if (data1.length < eeprom_useful_length) {
                debug_textbox.value += "ESC failed to reply with first data packet (too short, " + data1.length + " bytes)\r\n";
                serport_buttonsSetCurrent();
                return;
            }

            await serport_resetReader();
            await serport_tx(serport_genSetAddressCmd(eeprom_offset + eeprom_useful_length), 1
                , async function (data) { // sent read

                if (data == false || data == null || data.length < 1) {
                    debug_textbox.value += "ESC failed to ack 2nd address set command\r\n";
                    serport_buttonsSetCurrent();
                    return;
                }
                else if (data[0] != ack_byte) {
                    debug_textbox.value += "ESC failed to ack 2nd address set command (" + toHexString(data[0]) + ")\r\n";
                    serport_buttonsSetCurrent();
                    return;
                }

                await serport_resetReader();
                await serport_tx(serport_genReadCmd(eeprom_total_length - eeprom_useful_length), (eeprom_total_length - eeprom_useful_length) + 3
                    , async function (data) {

                    var data2 = data;
                    if (data2 == null || data2 == false) {
                        debug_textbox.value += "ESC failed to reply with second data packet\n";
                        serport_buttonsSetCurrent();
                        return;
                    }
                    else if (data2.length < (eeprom_total_length - eeprom_useful_length)) {
                        debug_textbox.value += "ESC failed to reply with second data packet (too short, " + data2.length + " bytes)\r\n";
                        serport_buttonsSetCurrent();
                        return;
                    }

                    var buffer = new ArrayBuffer(eeprom_total_length);
                    var buffer8 = new Uint8Array(buffer);
                    for (var i = 0; i < buffer8.length; i++)
                    {
                        if (i < eeprom_useful_length)
                        {
                            buffer8[i] = data1[i];
                        }
                        else if (i >= eeprom_useful_length && i <= eeprom_total_length)
                        {
                            buffer8[i] = data2[i - eeprom_useful_length];
                        }
                        else
                        {
                            buffer8[i] = 0xFF;
                        }
                    }
                    proc_cb(buffer8);
                }, 40, 2000); // sent read
            }); // sent set address
        }, 40, 2000); // sent read
    }); // sent set address
}

var serport_readMemoryRetries;

async function serport_readMemoryChunk(start_addr, len, proc_cb)
{
    if (serport_readMemoryRetries <= 0)
    {
        serport_buttonsSetCurrent();
        return;
    }
    serport_readMemoryRetries -= 1;

    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(start_addr), 1, async function (data) { // sent set address

        if (data == null || data == false || data.length < 1 || data[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command";
            if (serport_readMemoryRetries > 0) {
                debug_textbox.value += ", retrying...";
            }
            debug_textbox.value += "\r\n";
            serport_readMemoryChunk(start_addr, len, proc_cb);
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genReadCmd(len), len + 3, async function (data) { // sent read

            if (data == null || data == false) {
                debug_textbox.value += "ESC failed to reply data packet";
                if (serport_readMemoryRetries > 0) {
                    debug_textbox.value += ", retrying...";
                }
                debug_textbox.value += "\r\n";
                serport_readMemoryChunk(start_addr, len, proc_cb);
                return;
            }
            else if (data.length < len) {
                debug_textbox.value += "ESC failed to reply data packet (too short, " + data.length + " bytes)";
                if (serport_readMemoryRetries > 0) {
                    debug_textbox.value += ", retrying...";
                }
                debug_textbox.value += "\r\n";
                serport_readMemoryChunk(start_addr, len, proc_cb);
                return;
            }

            if (proc_cb) {
                proc_cb(data);
            }
        }, 40, 2000); // sent read
    }); // sent set address
}

function serport_reportQueryData(data)
{
    var mismatched = 0;
    var i;
    for (i = 0; i < data.length; i++) {
        debug_textbox.value += toHexString(data[i]) + " ";
        if (i >= device_id.length) {
            mismatched += 1;
        }
        else if (data[i] != device_id[i]) {
            mismatched += 1;
        }
    }
    for (; i < device_id.length; i++) {
        mismatched += 1;
    }
    debug_textbox.value += "\r\n";
    if (mismatched >= 2) {
        debug_textbox.value += "warning: device identifier might be wrong\r\n";
    }
}

async function serport_issueInitQuery(proc_cb)
{
    await serport_tx(serport_genInitQuery(), 9, async function (data) { // sent query

        if (data == null || data == false || data.length < 9) {
            debug_textbox.value += "ESC failed to reply to initial query\r\n";
            if (data != null && data != false && data.length > 0) {
                debug_textbox.value += "reply: ";
                serport_reportQueryData(data);
            }
            serport_buttonsSetCurrent();
            return;
        }

        debug_textbox.value += "ESC query reply: ";
        serport_reportQueryData(data);
        serport_hasQuery = true;

        if (proc_cb) {
            proc_cb();
        }
    }); // sent query
}

async function serport_readEeprom(proc_cb)
{
    if (serport == null) {
        return;
    }
    await serport_resetReader();
    if (serport_hasQuery == false)
    {
        await serport_issueInitQuery( async function () {
            await serport_readEepromInner(proc_cb);
        });
    }
    else
    {
        await serport_readEepromInner(proc_cb);
    }
}

var serport_writeRetry;
var serport_payloadRetry;

async function serport_writePayload(payload, start_addr, len, cb)
{
    if (serport_payloadRetry <= 0) {
        serport_buttonsSetCurrent();
        return;
    }
    serport_payloadRetry -= 1;

    await serport_resetReader();
    await serport_tx(serport_genPayload(payload, 0x00, len), 1
        , async function (data) { // sent payload

        if (data == null || data == false || data.length < 1) {
            debug_textbox.value += "ESC failed to ack payload";
            if (serport_writeRetry > 0) {
                debug_textbox.value += ", retrying...";
            }
            debug_textbox.value += "\r\n";
            await serport_writeBinaryInner(payload, start_addr, len, cb);
            return;
        }
        else if (data[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack payload (" + toHexString(data[0]) + " @ " + toHexString(start_addr) + ")";
            if (serport_payloadRetry > 0) {
                debug_textbox.value += ", retrying...";
            }
            debug_textbox.value += "\r\n";
            await serport_writePayload(payload, start_addr, len, cb);
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genFlashCmd(), 1
            , async function (data) { // sent flash command

            if (data == null || data == false || data.length < 1) {
                debug_textbox.value += "ESC failed to ack flash command";
                if (serport_writeRetry > 0) {
                    debug_textbox.value += ", retrying...";
                }
                debug_textbox.value += "\r\n";
                await serport_writeBinaryInner(payload, start_addr, len, cb);
                return;
            }
            else if (data[0] != ack_byte) {
                debug_textbox.value += "ESC failed to ack flash command (" + toHexString(data[0]) + ")";
                if (serport_writeRetry > 0) {
                    debug_textbox.value += ", retrying...";
                }
                debug_textbox.value += "\r\n";
                await serport_writeBinaryInner(payload, start_addr, len, cb);
                return;
            }

            if (cb) {
                cb();
            }

        }, 40, 2000); // sent flash command
    }, 40, 2000); // sent payload
}

async function serport_writeBinaryInner(payload, start_addr, len, cb)
{
    if (serport_writeRetry <= 0) {
        serport_buttonsSetCurrent();
        return;
    }
    serport_writeRetry -= 1;

    serport_payloadRetry = 3;
    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(start_addr), 1
        , async function (data) { // sent set address

        if (data == null || data == false || data.length < 1 || data[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command";
            if (serport_writeRetry > 0) {
                debug_textbox.value += ", retrying...";
            }
            debug_textbox.value += "\r\n";
            await serport_writeBinaryInner(payload, start_addr, len, cb);
            return;
        }
        else if (data[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command (" + toHexString(data[0]) + ")";
            if (serport_writeRetry > 0) {
                debug_textbox.value += ", retrying...";
            }
            debug_textbox.value += "\r\n";
            await serport_writeBinaryInner(payload, start_addr, len, cb);
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genSetBufferCmd(0x00, len), 0
            , async function (data) { // sent set buffer
            // ESC does not reply to this

            await serport_writePayload(payload, start_addr, len, cb);
        }); // sent set buffer
    }); // sent set address
}

async function serport_writeBinary(payload, start_addr, len, cb)
{
    if (serport == null) {
        return;
    }
    serport_writeRetry = 3;
    await serport_resetReader();
    if (serport_hasQuery == false)
    {
        await serport_issueInitQuery( async function () {
            await serport_writeBinaryInner(payload, start_addr, len, cb);
        });
    }
    else
    {
        await serport_writeBinaryInner(payload, start_addr, len, cb);
    }
}

var serport_fwChunks = [];
var serport_fwChunksVerify = [];
var serport_fwVerifyMe = null
var serport_fwIdx = 0;
var serport_fwCb = null;
var serport_fwVerified = false;

async function serport_writeFirmwareChunk()
{
    if (serport_fwChunks.length <= 0) {
        debug_textbox.value += "finished writing all FW chunks, starting verification...\r\n";
        serport_fwIdx = flash_fw_start;
        serport_verifyFirmwareChunk();
        return;
    }

    document.getElementById("txt_progress").innerHTML = serport_fwChunks.length + " chunks left to write";
    var nextChunk = serport_fwChunks[0];
    serport_fwChunksVerify.push(nextChunk);
    await serport_writeBinaryInner(nextChunk, serport_fwIdx + flash_start, nextChunk.length, async function() {
        await serport_writeFirmwareChunk();
    });
    serport_fwIdx += nextChunk.length;
    serport_fwChunks = serport_fwChunks.slice(1);
}

async function serport_verifyFirmwareChunk()
{
    if (serport_fwChunksVerify.length <= 0) {
        if (serport_fwCb) {
            serport_fwCb(serport_fwVerified);
        }
        return;
    }

    document.getElementById("txt_progress").innerHTML = serport_fwChunksVerify.length + " chunks left to verify";
    serport_fwVerifyMe = serport_fwChunksVerify[0];
    serport_readMemoryRetries = 3;
    await serport_readMemoryChunk(serport_fwIdx, serport_fwVerifyMe.length, async function(data) {
        var all_match = true;
        for (var i = 0; i < serport_fwVerifyMe.length; i++) {
            if (data[i] != serport_fwVerifyMe[i]) {
                debug_textbox.value += "FW verification failed at " + toHexString(serport_fwIdx - serport_fwVerifyMe.length + i) + ", " + toHexString(serport_fwVerifyMe[i]) + " != " + toHexString(data[i]) + "\r\n";
                all_match = false;
            }
        }
        if (all_match == false) {
            serport_fwVerified = false;
        }
        await serport_verifyFirmwareChunk();
    });
    serport_fwIdx += serport_fwVerifyMe.length;
    serport_fwChunksVerify = serport_fwChunksVerify.slice(1);
}

async function serport_readFirmwareChunk()
{
    const total_len = eeprom_offset + eeprom_total_length;
    if (serport_fwChunks.length >= total_len) {
        if (serport_fwCb) {
            serport_fwCb(serport_fwChunks);
        }
        return;
    }

    var percentage = 100 * serport_fwChunks.length / total_len;
    document.getElementById("txt_progress").innerHTML = Math.round(percentage) + "% read";

    var chunkLen = total_len - serport_fwIdx;
    if (chunkLen > 0x80) {
        chunkLen = 0x80;
    }

    serport_readMemoryRetries = 3;
    await serport_readMemoryChunk(serport_fwIdx, chunkLen, async function(data) {
        serport_fwChunks = uint8ArrayMerge(serport_fwChunks, data);
        serport_fwIdx += data.length;

        await serport_readFirmwareChunk();
    });
}

async function serport_writeFirmwareInner(payload, len, cb)
{
    serport_fwVerified = true;
    serport_fwCb = cb;
    serport_fwChunks = [];
    serport_fwChunksVerify = [];
    var i;
    for (i = 0; i < len; )
    {
        var chunkLen = len - i;
        if (chunkLen > (64-16)) {
            chunkLen = (64-16);
        }
        var chunk = payload.slice(i, i + chunkLen);
        serport_fwChunks.push(chunk);
        i += chunkLen;
    }
    serport_fwIdx = flash_fw_start;
    await serport_writeFirmwareChunk();
}

async function serport_writeFirmware(payload, len, cb)
{
    if (serport == null) {
        return;
    }
    await serport_resetReader();
    if (serport_hasQuery == false)
    {
        await serport_issueInitQuery( async function () {
            await serport_writeFirmwareInner(payload, len, cb);
        });
    }
    else
    {
        await serport_writeFirmwareInner(payload, len, cb);
    }
}

async function serport_readFirmware(cb)
{
    if (serport == null) {
        return;
    }
    serport_fwCb = cb;
    serport_fwChunks = new Uint8Array();
    serport_fwIdx = 0;
    await serport_resetReader();
    if (serport_hasQuery == false)
    {
        await serport_issueInitQuery( async function () {
            await serport_readFirmwareChunk(cb);
        });
    }
    else
    {
        await serport_readFirmwareChunk(cb);
    }
}

function serport_genSetAddressCmd(adr)
{
    var x = [0xFF, 0x00, 0x00, 0x00];
    x[2] = (adr & 0xFF00) >> 8;
    x[3] = (adr & 0x00FF) >> 0;
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
    return x;
}

function serport_genSetBufferCmd(x256, len)
{
    var x = [0xFE, 0x00, x256, len];
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
    return x;
}

function serport_genReadCmd(rdLen)
{
    var x = [0x03, rdLen];
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
    return x;
}

function serport_genInitQuery()
{
    var x = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '\r'.charCodeAt(0),
        'B' .charCodeAt(0),
        'L' .charCodeAt(0),
        'H' .charCodeAt(0),
        'e' .charCodeAt(0),
        'l' .charCodeAt(0),
        'i' .charCodeAt(0)];
    // I don't think this packet uses the same CRC calculation
    //var crc = serport_genCrc(x);
    //x.push((crc & 0x00FF) >> 0);
    //x.push((crc & 0xFF00) >> 8);
    // CRC should be 0xF4, 0x7D
    x.push(0xF4);
    x.push(0x7D);
    return x;
}

function serport_genPayload(bin, start, len)
{
    //var x = new Uint8Array(len + 2);
    var x = [];
    for (var i = 0; i < len; i++)
    {
        //x[i] = bin[start + i];
        x.push(bin[start + i]);
    }
    var crc = serport_genCrc(x);
    x[len    ] = ((crc & 0x00FF) >> 0);
    x[len + 1] = ((crc & 0xFF00) >> 8);
    return x;
}

function serport_genFlashCmd()
{
    var x = [0x01, 0x01];
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
    return x;
}

function serport_genRunCmd()
{
    var x = [0x00, 0x00, 0x00, 0x00];
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
    return x;
}

function serport_genCrc(barr)
{
    var crc16 = new Uint16Array(1);
    var xb = new Uint8Array(1);
    crc16[0] = 0;
    for (var i = 0; i < barr.length; i++)
    {
        xb[0] = barr[i] & 0xFF;
        for (var j = 0; j < 8; j++)
        {
            if (((xb[0] & 0x01) ^ (crc16[0] & 0x0001)) != 0) {
                crc16[0] = (crc16[0] >> 1) & 0xFFFF;
                crc16[0] = (crc16[0] ^ 0xA001) & 0xFFFF;
            }
            else {
                crc16[0] = (crc16[0] >> 1) & 0xFFFF;
            }
            xb[0] = (xb[0] >> 1) & 0xFF;
        }
    }
    return crc16[0] & 0xFFFF;
}

function serport_verifyCrc(barr)
{
    var contents = barr.slice(0, -2);
    var calculated_crc = serport_genCrc(contents);
    var received_crc = barr.slice(-2);
    received_crc = (received_crc[0]) + (received_crc[1] << 8);
    return calculated_crc == received_crc;
}

function serport_crcSelfTest()
{
    console.log("serport_crcSelfTest (true, false)");
    console.log(serport_verifyCrc([0xFF, 0x00, 0x7C, 0x00, 0x10, 0xD4]));
    console.log(serport_verifyCrc([0xFF, 0x00, 0x7C, 0x00, 0x10, 0xD3]));
}