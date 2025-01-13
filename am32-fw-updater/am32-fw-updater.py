import argparse
import os, time, datetime
from intelhex import IntelHex
import serial
from serial.tools.list_ports import comports

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("firmware",                                                       type=str, help="firmware file")
    parser.add_argument("serialport",                               default="auto",       type=str, help="serial port name")
    parser.add_argument("-a", "--fwaddr",     metavar="fwaddr",     default="0x08001000", type=str, help="firmware start address")
    parser.add_argument("-e", "--eepromaddr", metavar="eepromaddr", default="0x7C00",     type=str, help="eeprom address")
    parser.add_argument("-c", "--chunksize",  metavar="chunksize",  default="128",        type=str, help="chunk size")
    parser.add_argument("-m", "--mcu",        metavar="mcu",        default="",           type=str, help="MCU (overrides addresses)")
    parser.add_argument("-E", "--iseep",                            action="store_true",            help="is eeprom file")
    parser.add_argument("-i", "--inceep",                           action="store_true",            help="fw includes eeprom")
    parser.add_argument("-b", "--bindump",                          action="store_true",            help="fw bin is a dump")
    parser.add_argument("-v", "--verbose",                          action="store_true",            help="verbose")
    args = parser.parse_args()

    port_name = args.serialport
    if port_name == "auto" or args.verbose:
        ports = get_all_comports(args.verbose)
        if port_name == "auto":
            if len(ports) > 0:
                port_name = ports[0]
            else:
                print("no serial ports available")
                quit()

    fw_fullpath = os.path.abspath(args.firmware)
    fw_basename = os.path.basename(fw_fullpath)
    fw_namesplit = os.path.splitext(fw_basename)
    fw_justname = fw_namesplit[0].strip()
    fw_ext = fw_namesplit[1].strip().lower()
    if args.verbose:
        if args.iseep == False:
            print("firmware:")
        else:
            print("eeprom file:")
        print("\t" + fw_fullpath)
        print("\t%s (%s)" % (fw_basename, fw_ext))

    mcu = None
    addr_multi = 1
    if args.mcu is not None and len(args.mcu) > 0:
        mcu = args.mcu.strip().lower()
        if mcu == "f051":
            fwaddr   = 0x08001000
            eep_addr = 0x7C00
            addr_multi = 1
        elif mcu == "g071-64k":
            fwaddr   = 0x08001000
            eep_addr = 0xF800
            addr_multi = 1
        elif mcu == "g071-128k":
            fwaddr   = 0x08001000
            eep_addr = 0xF800
            addr_multi = 4
        elif mcu == "at421" or mcu == "f421":
            fwaddr   = 0x08001000
            eep_addr = 0x7C00
            addr_multi = 1
        else:
            raise Exception("Unknown (or unsupported) MCU specified")
        if args.verbose:
            print("MCU \"%s\": FW-addr 0x%08X ; EEP-addr 0x%04X ; addr-multi = %u" % (fwaddr, eep_addr, addr_multi))

    if mcu is None:
        if "0x" in args.fwaddr.lower():
            fwaddr = int(args.fwaddr, 16)
        else:
            try:
                fwaddr = int(args.fwaddr, 10)
            except:
                fwaddr = int(args.fwaddr, 16)
        if "0x" in args.eepromaddr.lower():
            eep_addr = int(args.eepromaddr, 16)
        else:
            try:
                eep_addr = int(args.eepromaddr, 10)
            except:
                eep_addr = int(args.eepromaddr, 16)

    if args.verbose:
        print("FW-addr 0x%08X ; EEP-addr 0x%04X ; addr-multi = %u" % (fwaddr, eep_addr, addr_multi))

    if args.iseep == False:
        if fw_ext == ".hex":
            fw_ihex = IntelHex(fw_fullpath)
        elif fw_ext == ".bin":
            fw_ihex = IntelHex()
            if args.bindump:
                adr = fwaddr & 0xFFFF0000
            else:
                adr = fwaddr
            with open(fw_fullpath, "rb") as f:
                x = f.read(1)
                while x != b"":
                    fw_ihex[adr] = x[0]
                    x = f.read(1)
                    adr += 1
        else:
            raise Exception("unknown firmware file type")
        if args.verbose:
            print("\tfrom 0x%08X to 0x%08X" % (fw_ihex.minaddr(), fw_ihex.maxaddr()))
    else:
        fw_ihex = IntelHex()
        if fw_ext == ".csv":
            with open(fw_fullpath, "r") as f:
                lines = f.readlines()
                i = 0
                for li in lines:
                    split = li.split(',')
                    if len(split) > 1:
                        last_str = split[-1].strip().lower()
                        try:
                            if "x" in last_str:
                                b = int(last_str, 16)
                            else:
                                b = int(last_str)
                        except:
                            b = 0
                        fw_ihex[i] = b
                        i += 1
            if args.verbose:
                print("CSV EEPROM loaded %u bytes (to 0x%04X)" % (i, fw_ihex.maxaddr()))
        elif fw_ext == ".bin":
            fw_ihex.loadbin(fw_fullpath)
            if args.verbose:
                print("raw binary EEPROM loaded %u bytes" % (fw_ihex.maxaddr() + 1))
        else:
            raise Exception("unknown eeprom file type, must be *.bin")

    if args.iseep == False:
        fw_binarr = fw_ihex.tobinarray(start = fwaddr)
    else: # is eeprom
        fw_binarr = fw_ihex.tobinarray(start = 0)

    if args.iseep == False and args.inceep == False:
        fw_binarr = fw_binarr[:eep_addr - (fwaddr & 0xFFFF)]

    if "0x" in args.chunksize.lower():
        chunksize = int(args.chunksize, 16)
    else:
        try:
            chunksize = int(args.chunksize, 10)
        except:
            chunksize = int(args.chunksize, 16)

    if (chunksize % 16) != 0:
        raise Exception("chunk size invalid, must be a multiple of 16")
    if chunksize > 256 or chunksize < 16:
        raise Exception("chunk size invalid, out of range (16 to 256)")
    if (1024 % chunksize) != 0:
        raise Exception("chunk size invalid, must reach page boundaries")
    if mcu is not None and mcu == "g071-128k" and (chunksize % 4) != 0:
        raise Exception("chunk size invalid, must be a multiple of 4")

    ser = serial.serial_for_url(port_name, do_not_open=True)
    ser.baudrate = 19200
    ser.bytesize = serial.EIGHTBITS
    ser.parity   = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    ser.timeout  = 1

    try:
        ser.open()
        if args.verbose:
            print("serial port opened")
        time.sleep(2)
    except serial.SerialException as e:
        print('Could not open serial port {}: {}\n'.format(ser.name, e))
        quit()

    i = 0
    if args.iseep == False:
        start_addr = fwaddr & 0xFFFF
    else:
        start_addr = eep_addr

    if args.verbose:
        print("array size = %u ; start addr = 0x%04X ; chunk size = %u" % (len(fw_binarr), start_addr, chunksize))

    j = start_addr
    done = False
    while i < len(fw_binarr) and done == False:
        thischunk = chunksize
        if (i + thischunk) >= len(fw_binarr):
            thischunk = len(fw_binarr) - i
            while (thischunk % 4) != 0:
                thischunk += 1
                fw_binarr.append(0xFF)
            done = True
        barr = fw_binarr[i:i + thischunk]

        if i != 0:
            print("\r", end="")
        print("writing to 0x%04X    " % j, end="")

        send_setaddress(ser, int(j / addr_multi))
        send_setbuffer(ser, j, thischunk)
        send_payload(ser, j, barr)
        send_flash(ser, j)
        i += thischunk
        j += thischunk

    print("\rfinished all writes, begin verification...")

    i = 0
    j = start_addr
    done = False
    while i < len(fw_binarr) and done == False:
        thischunk = chunksize
        if (i + thischunk) >= len(fw_binarr): # past the end of the firmware blob
            thischunk = len(fw_binarr) - i # set the chunk size to only be the remainder
            done = True
        barr = fw_binarr[i:i + thischunk]

        if i != 0:
            print("\r", end="")
        print("verifying 0x%04X    " % j, end="")

        tries = 3
        while tries > 0:
            send_setaddress(ser, int(j / addr_multi))
            data = send_readcmd(ser, j, thischunk)
            if len(barr) != len(data):
                raise Exception("verification read length at 0x%04X does not match, %u != %u" % (j, len(barr), len(data)))
            wcrc = crc16(barr)
            rcrc = crc16(data)
            tries -= 1
            if wcrc == rcrc:
                break
            if wcrc != rcrc and tries <= 0:
                raise Exception("verification read contents at 0x%04X does not match,\r\n\tdata %s\r\n\tread %s (%u %u %u)\r\n" % (j, format_arr(barr), format_arr(data), i, thischunk, len(fw_binarr)))
        i += thischunk
        j += thischunk
    print("\rfinished verification, all done")

def get_all_comports(to_print):
    if to_print:
        print("COM ports:")
    ports = []
    for n, (port, desc, hwid) in enumerate(sorted(comports()), 1):
        if to_print:
            print('--- {:2}: {:20} {!r}\n'.format(n, port, desc))
        ports.append(port)
    return ports

def crc16(data, length = None):
    if length is None:
        length = len(data)
    crc = 0
    i = 0
    while i < length:
        xb = data[i]
        j = 0
        while j < 8:
            if ((xb & 0x01) ^ (crc & 0x0001)) != 0:
                crc = (crc >> 1    ) & 0xFFFF
                crc = (crc ^ 0xA001) & 0xFFFF
            else:
                crc = (crc >> 1    ) & 0xFFFF
            xb = (xb >> 1) & 0xFF
            j += 1
        i += 1
    return crc & 0xFFFF

def append_crc(data):
    crc = crc16(data)
    data.append(crc & 0xFF)
    data.append(((crc & 0xFF00) >> 8) & 0xFF)
    return data

def read_serial(ser, rlen, timeout = 2):
    tstart = datetime.datetime.now()
    data = []
    while rlen > 0:
        x = ser.read(rlen)
        if len(x) > 0:
            data.extend(x)
            rlen -= len(x)
        tnow = datetime.datetime.now()
        if (tnow - tstart).total_seconds() > timeout:
            break
        time.sleep(0)
    return data

def serial_write(ser, data, rlen = -1, chunk_sz = 512, timeout = 2):
    tstart = datetime.datetime.now()
    ret = []
    while len(data) > 0:
        if len(data) > chunk_sz and chunk_sz > 0:
            this_chunk = data[0:chunk_sz]
        else:
            this_chunk = data
        ser.write(this_chunk)
        ser.flush()
        r = read_serial(ser, len(this_chunk))
        if len(r) > 0:
            ret.extend(r)
        else:
            ser.close()
            ser.open()
        if len(data) > chunk_sz and chunk_sz > 0:
            data = data[chunk_sz:]
        else:
            break
        tnow = datetime.datetime.now()
        if (tnow - tstart).total_seconds() > timeout:
            break
        time.sleep(0.01)
    if rlen >= 0 and len(ret) < rlen:
        r = read_serial(ser, rlen - len(ret))
        if len(r) > 0:
            ret.extend(r)
    return ret

def send_setaddress(ser, addr):
    try:
        x = bytearray([0xFF, 0x00, 0x00, 0x00])
        x[2] = (addr & 0xFF00) >> 8;
        x[3] = (addr & 0x00FF) >> 0;
        x = append_crc(x)
        y = serial_write(ser, x, len(x) + 1)
        if len(y) < len(x) + 1:
            raise Exception("did not read enough data, len %u < %u" % (len(y), len(x) + 1))
        if y[-1] != 0x30:
            raise Exception("did not get valid ack, 0x%02X" % y[-1])
        time.sleep(0.001) # this is only here to put a gap in the logic analyzer display, so I can decipher the stream visially
    except Exception as ex:
        print("ERROR during set address command (@ 0x%04X), exception: %s" % (addr, ex))
        quit()

def send_setbuffer(ser, addr, buflen):
    try:
        x = bytearray([0xFE, 0x00, 0, buflen])
        x = append_crc(x)
        y = serial_write(ser, x, len(x))
        if len(y) < len(x):
            raise Exception("did not read enough data, len %u < %u" % (len(y), len(x)))
        time.sleep(0.0035) # requires a long timeout, the bootloader code is reliant on a timeout before running the parser, but we have no ACK to indicate if this has occured, so we must hard-code a delay
    except Exception as ex:
        print("ERROR during set buffer command (@ 0x%04X %u), exception: %s" % (addr, buflen, ex))
        quit()

def send_payload(ser, addr, x):
    try:
        x = append_crc(x)
        y = serial_write(ser, x, len(x) + 1)
        if len(y) < len(x) + 1:
            raise Exception("did not read enough data, len %u < %u" % (len(y), len(x) + 1))
        if y[-1] != 0x30:
            raise Exception("did not get valid ack, 0x%02X" % y[-1])
    except Exception as ex:
        print("ERROR during payload transfer (@ 0x%04X), exception: %s" % (addr, ex))
        quit()

def send_flash(ser, addr):
    try:
        x = bytearray([0x01, 0x01])
        x = append_crc(x)
        y = serial_write(ser, x, len(x) + 1)
        if len(y) < len(x) + 1:
            raise Exception("did not read enough data, len %u < %u" % (len(y), len(x) + 1))
        if y[-1] != 0x30:
            raise Exception("did not get valid ack, 0x%02X" % y[-1])
    except Exception as ex:
        print("ERROR during flash command (@ 0x%04X), exception: %s" % (addr, ex))
        quit()

def send_readcmd(ser, addr, buflen):
    try:
        x = bytearray([0x03, buflen])
        x = append_crc(x)
        expected = len(x) + buflen + 3
        y = serial_write(ser, x, expected)
        if y[-1] != 0x30:
            raise Exception("did not get valid ack, 0x%02X" % y[-1])
        y = y[len(x):]
        data = y[:-3]
        if len(data) != buflen:
            raise Exception("length of read data does not match, %u != %u" % (len(data), buflen))
        #calcedcrc = crc16(data)
        #rxedcrc = (y[-2] << 8) | y[-3]
        #if rxedcrc != calcedcrc:
        #    print("\nWARNING: CRC mismatch @ 0x%04X, rx 0x%04X != calc 0x%04X" % (addr, rxedcrc, calcedcrc))
        return data
    except Exception as ex:
        print("ERROR during read command (@ 0x%04X %u), exception: %s" % (addr, buflen, ex))
        quit()

def format_arr(data):
    s = "["
    for i in data:
        s += "%02X " % i
    s = s.strip()
    s += "]"
    return s

if __name__ == '__main__':
    main()