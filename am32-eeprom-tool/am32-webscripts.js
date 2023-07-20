function text_to_id(t)
{
    return t.toLowerCase().replaceAll(" ", "").replaceAll("-", "").replaceAll(".", "");
}

function make_checkbox(x)
{
    var t;
    t = "<div style=\"display: table-cell; text-align: right;\"><label for=\"chk_" + text_to_id(x[0]) + "\">" + x[0] + "</label></div>\r\n";
    t += "<div style=\"display: table-cell; text-align: left\"><input id=\"chk_" + text_to_id(x[0]) + "\" type=\"checkbox\"";
    if (x[1])
    {
        t += " checked=\"checked\"";
    }
    t += " /></div>\r\n";
    if (x[2] >= eeprom_useful_length) {
        debug_textbox.value += "warning: slider \"" + x[0] + "\"'s EEPROM index is over the EEPROM expected length\r\n";
    }
    return t;
}

function make_slider(x, i)
{
    var t;
    t = "<div style=\"display: table-cell; text-align: right; padding-right:5pt;\"><label for=\"txt_" + text_to_id(x[0]) + "\">" + x[0] + "</label></div>\r\n";
    t += "<div style=\"display: table-cell; text-align: left; padding-right: 10pt;\"><input id=\"txt_" + text_to_id(x[0]) + "\" type=\"number\" style=\"width:100%; text-align: right;\"";
    t += "min=\"" + x[2] + "\" max=\"" + x[3] + "\" value=\"" + x[1] + "\" step=\"" + x[4] + "\" ";
    t += "onchange=\"txt_onchange(" + i + ")\"";
    t += " /></div>\r\n";
    t += "<div style=\"display: table-cell;\"><input id=\"sld_" + text_to_id(x[0]) + "\" type=\"range\" ";
    t += "min=\"" + x[2] + "\" max=\"" + x[3] + "\" value=\"" + x[1] + "\" step=\"" + x[4] + "\" ";
    t += "onchange=\"sld_onchange(" + i + ")\"";
    t += " oninput=\"sld_onchange(" + i + ")\"";
    t += " /></div>\r\n";
    t += "<div style=\"display: table-cell; padding-left:5pt;\">def = " + x[1] + "</div>\r\n";
    if (x[6] >= eeprom_useful_length) {
        debug_textbox.value += "warning: slider \"" + x[0] + "\"'s EEPROM index is over the EEPROM expected length\r\n";
    }
    return t;
}

function make_all_checkboxes()
{
    var t = "";
    for (var i = 0; i < all_checkboxes.length; i++)
    {
        t += "<div style=\"display: table-row;\">\r\n";
        t += make_checkbox(all_checkboxes[i]);
        t += "</div>\r\n";
    }
    return t;
}

function make_all_sliders()
{
    var t = "";
    for (var i = 0; i < all_sliders.length; i++)
    {
        t += "<div style=\"display: table-row;\">\r\n";
        t += make_slider(all_sliders[i], i);
        t += "</div>\r\n";
    }
    return t;
}

var ui_locked = false;

function txt_onchange(i)
{
    if (ui_locked) {
        return;
    }
    ui_locked = true;
    var sld = all_sliders[i];
    var v = document.getElementById("txt_" + text_to_id(sld[0])).value;
    if (v < sld[2])
    {
        v = sld[2];
    }
    if (v > sld[3])
    {
        v = sld[3];
    }
    document.getElementById("sld_" + text_to_id(sld[0])).value = v;
    ui_locked = false;
}

function sld_onchange(i)
{
    if (ui_locked) {
        return;
    }
    ui_locked = true;
    var sld = all_sliders[i];
    var v = document.getElementById("sld_" + text_to_id(sld[0])).value;
    if (v < sld[2])
    {
        v = sld[2];
    }
    if (v > sld[3])
    {
        v = sld[3];
    }
    document.getElementById("txt_" + text_to_id(sld[0])).value = v;
    ui_locked = false;
}

function readBinFile(e)
{
    debug_textbox.value = "";
    var file = e.target.files[0];
    if (!file) {
        return;
    }
    var reader = new FileReader();
    reader.onload = function(e) {
        var barr = new Uint8Array(e.target.result);
        readBin(barr);
        debug_textbox.value += "Local binary file loaded\r\n";
        document.getElementById("btn_readbinfile").value = "";
    };
    reader.readAsArrayBuffer(file);
}

function readBin(barr)
{
    var dbg_txt = "";

    if (barr[0] == 0 || barr[0] == 0xFF) {
        dbg_txt += "warning: EEPROM appears empty\r\n";
    }
    if (barr[1] == 0 || barr[1] == 0xFF) {
        dbg_txt += "warning: EEPROM has no layout indicator\r\n";
    }
    if (barr[2] == 0 || barr[2] == 0xFF) {
        dbg_txt += "warning: bootloader version invalid\r\n";
    }

    for (var i = 0; i < all_checkboxes.length; i++)
    {
        var c = all_checkboxes[i];
        var ele = document.getElementById("chk_" + text_to_id(c[0]));
        ele.checked = barr[c[2]] != 0;
    }

    for (var i = 0; i < all_sliders.length; i++)
    {
        var sld = all_sliders[i];
        var bidx = sld[6];
        var eles = document.getElementById("sld_" + text_to_id(sld[0]));
        var elet = document.getElementById("txt_" + text_to_id(sld[0]));
        var val = barr[bidx];
        val *= sld[4];
        val += sld[5];
        if (val < sld[2] || val > sld[3])
        {
            dbg_txt += "\"" + sld[0] + "\" value " + val + " is out of range\r\n";
            if (val < sld[2])
            {
                val = sld[2];
            }
            if (val > sld[3])
            {
                val = sld[3];
            }
        }
        elet.value = val;
        txt_onchange(i);
    }

    var drop_rcinput = document.getElementById("drop_rcinput");
    drop_rcinput.value = "x_" + barr[46].toString();

    var txt_devicename = document.getElementById("txt_devicename");
    var dev_name = "";
    for (var i = 0; i < 12; i++)
    {
        var x = barr[5 + i];
        if (x == 0 || x == 0xFF) {
            break;
        }
        dev_name += String.fromCharCode(x);
    }
    txt_devicename.value = dev_name;

    if (dbg_txt.length > 0) {
        debug_textbox.value += dbg_txt + "\r\n";
    }
}

function generateBin()
{
    var dbg_txt = "";
    let buffer = new ArrayBuffer(176);
    let buffer8 = new Uint8Array(buffer);
    for (var i = 0; i < buffer8.length; i++)
    {
        buffer8[i] = 0xFF;
    }
    buffer8[0] = 1; // indicate filled
    buffer8[1] = 1; // EEPROM layout version
    for (var i = 0; i < all_checkboxes.length; i++)
    {
        var c = all_checkboxes[i];
        var ele = document.getElementById("chk_" + text_to_id(c[0]));
        buffer8[c[2]] = ele.checked ? 1 : 0;
    }
    for (var i = 0; i < all_sliders.length; i++)
    {
        var sld = all_sliders[i];
        var ele = document.getElementById("txt_" + text_to_id(sld[0]));
        var val = ele.value;
        val -= sld[5];
        val /= sld[4];
        if (val < 0 || val > 255) {
            dbg_txt += "\"" + sld[0] + "\" byte value " + val + " is overflowing\r\n";
            if (val < 0) {
                val = 0;
            }
            if (val > 255) {
                val = 255;
            }
        }
        buffer8[sld[6]] = Math.round(val);
    }

    var drop_rcinput = document.getElementById("drop_rcinput");
    buffer8[46] = Math.round(parseInt(drop_rcinput.value.substring(2)));

    var txt_devicename = document.getElementById("txt_devicename");
    var i;
    for (i = 0; i < 12 && i < txt_devicename.value.length; i++)
    {
        buffer8[5 + i] = Math.round(txt_devicename.value.charCodeAt(i)) & 0xFF;
    }
    for (; i < 12; i++)
    {
        buffer8[5 + i] = 0;
    }

    if (dbg_txt.length > 0) {
        debug_textbox.value += dbg_txt + "\r\n";
    }

    return buffer8;
}

function getBinFileName(fname, default_name)
{
    if (fname.length > 0) {
        if (filename_isValid(fname)) {
            if (fname.toLowerCase().endsWith(".bin") == false) {
                fname += ".bin";
            }
            while (fname.includes("..")) {
                fname = fname.replaceAll("..", ".");
            }
        }
        else {
            debug_textbox.value = "warning: invalid save file name, using default name instead\r\n";
            fname = "";
        }
    }
    if (fname.length <= 0) {
        fname = default_name;
    }
    return fname;
}

function saveBinFile()
{
    debug_textbox.value = "";
    var fname = document.getElementById("txt_savefname").value;
    saveByteArray(generateBin(), getBinFileName(fname, "am32-eeprom.bin"));
}

var filename_isValid=(function(){
  var rg1=/^[^\\/:\*\?"<>\|]+$/; // forbidden characters \ / : * ? " < > |
  var rg2=/^\./; // cannot start with dot (.)
  var rg3=/^(nul|prn|con|lpt[0-9]|com[0-9])(\.|$)/i; // forbidden file names
  return function filename_isValid(fname){
    return rg1.test(fname)&&!rg2.test(fname)&&!rg3.test(fname);
  }
})();

function toHexString(x)
{
    var hex = x.toString(16);
    while ((hex.length % 2) != 0) {
        hex = "0" + hex;
    }
    return "0x" + hex.toUpperCase();
}

function uint8ArrayMerge(x, y)
{
    if (x == null)
    {
        x = new Uint8Array(y.length);
        for (var i = 0; i < y.length; i++) {
            x[i] = y[i];
        }
    }
    else
    {
        var mergedArray = new Uint8Array(x.length + y.length);
        mergedArray.set(x);
        for (var i = 0, j = x.length; i < y.length && j < mergedArray.length; i++, j++) {
            mergedArray[j] = y[i];
        }
        x = mergedArray; 
    }
    return x;
}

function serport_buttonsSetDisabled(disabled)
{
    document.getElementById("btn_serread"   ).disabled = disabled;
    document.getElementById("btn_serwrite"  ).disabled = disabled;
    document.getElementById("btn_sererase"  ).disabled = disabled;
    document.getElementById("btn_serrefresh").disabled = disabled;
    document.getElementById("btn_serrunapp" ).disabled = disabled;
    document.getElementById("btn_fwupdate"  ).disabled = disabled;
    document.getElementById("btn_fwdump"    ).disabled = disabled;
}

function serport_buttonsSetCurrent()
{
    document.getElementById("div_progress").style.display = "none";
    if (serport == null) {
        serport_buttonsSetDisabled(true);
    }
    else {
        serport_buttonsSetDisabled(false);
    }
}

function serport_btnRead()
{
    serport_buttonsSetDisabled(true);
    debug_textbox.value = "";
    serport_readEeprom(function(barr) {
        readBin(barr);
        debug_textbox.value += "EEPROM data read from ESC\r\n";
        serport_buttonsSetCurrent();
    });
}

function serport_writeAndVerify(barr_sent, message)
{
    serport_buttonsSetDisabled(true);
    debug_textbox.value = "";
    serport_writeBinary(barr_sent, eeprom_offset, eeprom_useful_length, function() {
        serport_readEeprom(function(barr_read) {
            if (barr_read.length < 
            //barr_sent.length
            eeprom_useful_length
            ) {
                debug_textbox.value += "EEPROM verification length too short\r\n";
            }
            for (var i = 0;
                i < barr_sent.length && i < barr_read.length &&
                i < eeprom_useful_length
                ; i++) {
                if (barr_sent[i] != barr_read[i]) {
                    debug_textbox.value += "EEPROM verification mismatch at " + i + "\r\n";
                }
            }
            debug_textbox.value += message + "\r\n";
            serport_buttonsSetCurrent();
        });
    });
}

function serport_btnWrite()
{
    serport_writeAndVerify(generateBin(), "EEPROM data written to ESC");
}

function serport_btnErase()
{
    var barr_sent = new Uint8Array(eeprom_total_length);
    for (var i = 0; i < eeprom_total_length; i++) { barr_sent[i] = 0xFF; }
    serport_writeAndVerify(barr_sent, "EEPROM data erased from ESC");
}

function serport_btnRefresh()
{
    var barr_sent = generateBin();
    barr_sent[3] = 0; // clearing the version bytes will force the firmware to rewrite the EEPROM with correct version info
    barr_sent[4] = 0;
    serport_writeAndVerify(barr_sent, "ESC FW numbers erased, run the FW to have it be refreshed, then read the EERPROM");
}

function serport_btnRunApp()
{
    serport_tx(serport_genRunCmd(), 0, async function (data) {
        debug_textbox.value += "ESC may have rebooted\r\n";
    });
}

function serport_fwupdate(e)
{
    serport_buttonsSetDisabled(true);
    debug_textbox.value = "";
    var file = e.target.files[0];
    if (!file) {
        return;
    }
    var reader = new FileReader();
    reader.onload = function(e) {
        try {
            document.getElementById("div_progress").style.display = "block";
            document.getElementById("txt_progress").innerHTML = "preparing file...";
            let memMap = MemoryMap.fromHex(e.target.result);
            var start_addr = flash_start + flash_fw_start;
            var total_len = eeprom_offset - flash_fw_start;
            var end_addr = start_addr + total_len;
            let fwArr = memMap.slicePad(start_addr, total_len);
            debug_textbox.value += "writing from " + toHexString(start_addr) + " , len = " + toHexString(total_len) + " (" + total_len + ") , ending at " + toHexString(end_addr) + "\r\n";
            serport_writeFirmware(fwArr, fwArr.length, function(good) {
                if (good) {
                    debug_textbox.value += "finished verifying all FW chunks\r\n";
                }
                else {
                    debug_textbox.value += "finished attempting to verify FW, has failures\r\n";
                }
                serport_buttonsSetCurrent();
            });
        }
        catch (ex) {
            debug_textbox.value += "error: exception while reading/sending firmware: " + ex.toString();
        }
        document.getElementById("btn_fwupdate").value = "";
    };
    reader.readAsText(file);
}

function serport_fwdump()
{
    let fname = prompt("Dump file name?");
    if (fname == null) {
        return;
    }
    fname = getBinFileName(fname, "am32-fw.bin");
    serport_buttonsSetDisabled(true);
    document.getElementById("div_progress").style.display = "block";
    document.getElementById("txt_progress").innerHTML = "";
    debug_textbox.value = "";
    serport_readFirmware(function(data) {
        debug_textbox.value += "finished reading FW bin\r\n";
        saveByteArray(data, fname);
        serport_buttonsSetCurrent();
    });
}
