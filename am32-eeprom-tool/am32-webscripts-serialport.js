var serport = null;
var serport_reader = null;
var serport_writer = null;

const pkt_time = 300;
const ack_byte = 0x30;

var serport_fifo = null;
var serport_echocnt = 0;
var serport_hasQuery = false;

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

var serport_txEchoReadAttempts;
var serport_txEchoBuffer;

async function serport_tx(data, cb)
{
    if (serport == null) {
        return;
    }
    serport_txEchoReadAttempts = 0;
    var buffer = new ArrayBuffer(data.length);
    var buffer8 = new Uint8Array(buffer);
    for (var i = 0; i < data.length; i++) { buffer8[i] = data[i]; }
    console.log("serport sending");
    console.log(buffer8);
    await serport_writer.write(buffer8);
    serport_echocnt = buffer8.length;
    serport_txEchoBuffer = buffer8;
    setTimeout(async function () {
        await serport_txReadEcho(cb);
    }, pkt_time);
}

async function serport_txReadEcho(cb)
{
    if (serport == null) {
        debug_textbox.value += "error: serial port is null\r\n";
        return;
    }

    var r = 0;
    while (serport.readable)
    {
        try
        {
            let { value, done } = await Promise.race([
                    serport_reader.read(),
                    new Promise((_, reject) => setTimeout(reject, pkt_time, new Error("serport_tx read timeout")))
                    ]);
            if (value != null)
            {
                console.log("serport replied");
                console.log(value);
                if (value.length > 0)
                {
                    r += value.length;
                    serport_fifoPush(value);
                    console.log(serport_fifo);
                    done = (r >= serport_txEchoBuffer.length);
                }
            }
            if (done)
            {
                if (serport_fifo.length >= serport_txEchoBuffer.length)
                {
                    for (var i = 0; i < serport_txEchoBuffer.length; i++) {
                        if (serport_fifo[i] != serport_txEchoBuffer[i]) {
                            debug_textbox.value += "warning: echo content does not match sent data\r\n";
                            break;
                        }
                    }
                }
                if (serport_echocnt > 0) {
                    serport_fifoPopBlindCnt(serport_echocnt);
                    serport_echocnt = 0;
                    console.log(serport_fifo);
                }
                break;
            }
        }
        catch (ex)
        {
            break;
        }
    }

    if (serport_txEchoReadAttempts <= 0) {
        if (cb) {
            console.log("serport tx calling cb");
            cb();
        }
    }
    else {
        serport_txEchoReadAttempts -= 1;
        setTimeout(async function () {
            await serport_txReadEcho(cb);
        }, pkt_time);
    }
}

async function serport_readToEnd()
{
    if (serport == null) {
        debug_textbox.value += "error: serial port is null\r\n";
        return;
    }

    if (serport_fifo != null)
    {
        if (serport_fifo.length > 0)
        {
            if (serport_echocnt > 0) {
                serport_fifoPopBlindCnt(serport_echocnt);
                serport_echocnt = 0;
            }
            console.log("serport readtoend early");
            console.log(serport_fifo);
            return serport_fifo;
        }
    }

    if (serport.readable)
    {
        try
        {
            let { value, done } = await Promise.race([
                serport_reader.read(),
                new Promise((_, reject) => setTimeout(function() {
                    console.log("serport_readToEnd timeout");
                    if (serport_fifo == null || serport_fifo.length <= 0) {
                        //debug_textbox.value += "error: serial port timed out reading\r\n";
                    }
                    var empty = [];
                    return [ empty, true ];
                }, pkt_time))
            ]);
            serport_fifoPush(value);
            if (serport_echocnt > 0) {
                serport_fifoPopBlindCnt(serport_echocnt);
                serport_echocnt = 0;
            }
            console.log("serport_readToEnd returning");
            console.log(serport_fifo);
        }
        catch
        {
            debug_textbox.value += "error: serial port unable to read\r\n";
        }
    }
    else {
        debug_textbox.value += "error: serial port is not readable\r\n";
    }
    console.log("serport readtoend");
    console.log(serport_fifo);
    return serport_fifo;
}

async function serport_readBinaryInner(proc_cb)
{
    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(eeprom_offset)
        // [0xFF, 0x00, 0x7C, 0x00, 0x10, 0xD4]
        , async function () { // sent set address

        var tmp = await serport_readToEnd();
        if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command\r\n";
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genReadCmd(eeprom_useful_length)
            //[0x03, 0x30, 0x00, 0xE4]
            , async function () { // sent read

            var data1 = await serport_readToEnd();
            if (data1 == null) { // || data1.length < eeprom_useful_length) {
                debug_textbox.value += "ESC failed to reply first data packet\r\n";
                return;
            }
            else if (data1.length < eeprom_useful_length) {
                await serport_resetReader();
                var data3 = await serport_readToEnd();
                data1 = uint8ArrayMerge(data1, data3);
                if (data1.length < eeprom_useful_length) {
                    debug_textbox.value += "ESC failed to reply with first data packet (too short, " + data1.length + " bytes)\r\n";
                    return;
                }
            }

            await serport_resetReader();
            await serport_tx(serport_genSetAddressCmd(eeprom_offset + eeprom_useful_length)
                // [0xFF, 0x00, 0x7C, 0x30, 0x10, 0xC0]
                , async function () { // sent read

                var tmp = await serport_readToEnd();
                if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
                    debug_textbox.value += "ESC failed to ack 2nd address set command\r\n";
                    return;
                }

                await serport_resetReader();
                await serport_tx(serport_genReadCmd(eeprom_total_length - eeprom_useful_length)
                    // [0x03, 0x80, 0x01, 0x50]
                    , async function () {

                    var data2 = await serport_readToEnd();
                    if (data2 == null) { // || data2.length < (eeprom_total_length - eeprom_useful_length)) {
                        debug_textbox.value += "ESC failed to reply with second data packet\n";
                        return;
                    }
                    else if (data2.length < (eeprom_total_length - eeprom_useful_length)) {
                        await serport_resetReader();
                        var data3 = await serport_readToEnd();
                        data2 = uint8ArrayMerge(data2, data3);
                        if (data2.length < (eeprom_total_length - eeprom_useful_length)) {
                            debug_textbox.value += "ESC failed to reply with second data packet (too short, " + data2.length + " bytes)\r\n";
                            return;
                        }
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
                }); // sent read
            }); // sent set address
        }); // sent read
    }); // sent set address
}

async function serport_readMemoryChunk(start_addr, len, proc_cb)
{
    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(start_addr), async function () { // sent set address

        var tmp = await serport_readToEnd();
        if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command\r\n";
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genReadCmd(len), async function () { // sent read

            var data = await serport_readToEnd();
            if (data == null) {
                debug_textbox.value += "ESC failed to reply data packet\r\n";
                return;
            }
            else if (data.length < len) {
                await serport_resetReader();
                var data2 = await serport_readToEnd();
                data = uint8ArrayMerge(data, data2);
                if (data.length < len) {
                    debug_textbox.value += "ESC failed to reply data packet (too short, " + data.length + " bytes)\r\n";
                    return;
                }
            }

            if (proc_cb) {
                proc_cb(data);
            }
        }); // sent read
    }); // sent set address
}

async function serport_issueInitQuery(proc_cb)
{
    await serport_tx(serport_genInitQuery(), async function () { // sent query

        var tmp = await serport_readToEnd();
        if (tmp == null || tmp.length < 9) {
            debug_textbox.value += "ESC failed to reply to initial query\r\n";
            if (tmp != null && tmp.length > 0) {
                debug_textbox.value += "reply: ";
                var mismatched = 0;
                for (var i = 0; i < tmp.length; i++) {
                    debug_textbox.value += toHexString(tmp[i]) + " ";
                    if (i >= device_id.length) {
                        mismatched += 1;
                    }
                    else if (tmp[i] != device_id[i]) {
                        mismatched += 1;
                    }
                }
                debug_textbox.value += "\r\n";
                if (mismatched >= 2) {
                    debug_textbox.value += "warning: device identifier might be wrong\r\n";
                }
            }
            return;
        }

        debug_textbox.value += "ESC query reply: ";
        for (var i = 0; i < serport_fifo.length; i++) {
            debug_textbox.value += toHexString(serport_fifo[i]) + " ";
        }
        debug_textbox.value += "\r\n";
        serport_hasQuery = true;

        if (proc_cb) {
            await proc_cb();
        }
    }); // sent query
}

async function serport_readBinary(proc_cb)
{
    if (serport == null) {
        return;
    }
    await serport_resetReader();
    if (serport_hasQuery == false)
    {
        await serport_issueInitQuery( async function () {
            await serport_readBinaryInner(proc_cb);
        });
    }
    else
    {
        await serport_readBinaryInner(proc_cb);
    }
}

async function serport_writeBinaryInner(payload, start_addr, len, cb)
{
    await serport_resetReader();
    await serport_tx(serport_genSetAddressCmd(start_addr)
        // [0xFF, 0x00, 0x7C, 0x00, 0x10, 0xD4]
        , async function () { // sent set address

        var tmp = await serport_readToEnd();
        if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
            debug_textbox.value += "ESC failed to ack address set command\r\n";
            return;
        }

        await serport_resetReader();
        await serport_tx(serport_genSetBufferCmd(0x00, len)
            // [0xFE, 0x00, 0x00, 0x30, 0x31, 0xFC]
            , async function () { // sent set buffer
            // ESC does not reply to this

            await serport_resetReader();
            // expected to send (0x30 + 2) bytes
            await serport_tx(serport_genPayload(payload, 0x00, len)
                , async function () { // sent payload

                var tmp = await serport_readToEnd();
                if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
                    debug_textbox.value += "ESC failed to ack payload\r\n";
                    return;
                }

                await serport_resetReader();
                await serport_tx(serport_genFlashCmd()
                    , async function () { // sent flash command

                    var tmp = await serport_readToEnd();
                    if (tmp == null || tmp.length < 1 || tmp[0] != ack_byte) {
                        debug_textbox.value += "ESC failed to ack flash command\r\n";
                        return;
                    }

                    if (cb) {
                        cb();
                    }

                }); // sent flash command
            }); // sent payload
        }); // sent set buffer
    }); // sent set address
}

async function serport_writeBinary(payload, start_addr, len, cb)
{
    if (serport == null) {
        return;
    }
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
        debug_textbox.value += "finished writing all FW chunks\r\n";
        serport_fwIdx = flash_fw_start;
        serport_verifyFirmwareChunk();
        return;
    }

    var nextChunk = serport_fwChunks[0];
    serport_fwChunksVerify.push(nextChunk);
    await serport_writeBinaryInner(nextChunk, serport_fwIdx, nextChunk.length, async function() {
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
    serport_fwVerifyMe = serport_fwChunksVerify[0];
    await serport_readMemoryChunk(serport_fwIdx, serport_fwVerifyMe.length, async function(data) {
        var all_match = true;
        for (var i = 0; i < serport_fwVerifyMe.length; i++) {
            if (data[i] != serport_fwVerifyMe[i]) {
                debug_textbox.value += "FW verification failed at 0x" + toHexString(serport_fwIdx - serport_fwVerifyMe.length + i) + ", 0x" + toHexString(serport_fwVerifyMe[i]) + " != 0x" + toHexString(data[i]) + "\r\n";
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

    var chunkLen = total_len - serport_fwIdx;
    if (chunkLen > 0x80) {
        chunkLen = 0x80;
    }

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
        if (chunkLen > 0x80) {
            chunkLen = 0x80;
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
    var x = [];
    for (var i = 0; i < len; i++)
    {
        x.push(bin[start + i]);
    }
    var crc = serport_genCrc(x);
    x.push((crc & 0x00FF) >> 0);
    x.push((crc & 0xFF00) >> 8);
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
    var crc16 = 0;
    for (var i = 0; i < barr.length; i++)
    {
        var xb = barr[i];
        for (var j = 0; j < 8; j++)
        {
            if (((xb & 0x01) ^ (crc16 & 0x0001)) != 0) {
                crc16 = crc16 >> 1;
                crc16 = crc16 ^ 0xA001;
            }
            else {
                crc16 = crc16 >> 1;
            }
            xb = xb >> 1;
        }
    }
    return crc16;
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