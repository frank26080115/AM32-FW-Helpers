import argparse
import os
from intelhex import IntelHex

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("bootloader", type=str, help="bootloader file")
    parser.add_argument("firmware", type=str, help="firmware file")
    parser.add_argument("-r", "--replace", action="store_true", help="replace instead of ignore during merge")
    parser.add_argument("-n", "--fresh", action="store_true", help="make the EEPROM look fresh")
    parser.add_argument("-a", "--baseaddr", metavar="baseaddr", default="0x08000000", type=str, help="base address")
    parser.add_argument("-e", "--eeprom", metavar="eeprom", default=None, type=str, help="eeprom file")
    parser.add_argument("-x", "--eepromaddr", metavar="eepromaddr", default="0x7C00", type=str, help="eeprom address")
    parser.add_argument("-m", "--mcu", metavar="mcu", default="", type=str, help="MCU (overrides addresses)")
    parser.add_argument("--singlechunk", action="store_true", help="make sure output file is a single chunk hex file")
    parser.add_argument("-o", "--outpath", metavar="outpath", default=None, type=str, help="output file path")
    parser.add_argument("-v", "--verbose", action="store_true", help="verbose")
    args = parser.parse_args()

    if "0x" in args.baseaddr.lower():
        base_address = int(args.baseaddr, 16)
    else:
        try:
            base_address = int(args.baseaddr, 10)
        except:
            base_address = int(args.baseaddr, 16)

    if args.verbose:
        print("base address 0x%08X" % base_address)

    bl_justname = None
    if args.bootloader != 'x':
        bl_fullpath = os.path.abspath(args.bootloader)
        bl_basename = os.path.basename(bl_fullpath)
        bl_namesplit = os.path.splitext(bl_basename)
        bl_justname = bl_namesplit[0].strip()
        bl_ext = bl_namesplit[1].strip().lower()
        if args.verbose:
            print("bootloader:")
            print("\t" + bl_fullpath)
            print("\t%s (%s)" % (bl_basename, bl_ext))

        if bl_ext == ".bin":
            bl_ihex = IntelHex()
            bl_ihex.loadbin(bl_fullpath, offset = base_address)
        elif bl_ext == ".hex":
            bl_ihex = IntelHex(bl_fullpath)
        else:
            raise Exception("unknown bootloader file type, must be *.bin or *.hex")

        if args.verbose:
            print("bootloader from 0x%08X to 0x%08X" % (bl_ihex.minaddr(), bl_ihex.maxaddr()))

    fw_justname = None
    if args.firmware != 'x':
        fw_fullpath = os.path.abspath(args.firmware)
        fw_basename = os.path.basename(fw_fullpath)
        fw_namesplit = os.path.splitext(fw_basename)
        fw_justname = fw_namesplit[0].strip()
        fw_ext = fw_namesplit[1].strip().lower()
        if args.verbose:
            print("firmware:")
            print("\t" + fw_fullpath)
            print("\t%s (%s)" % (fw_basename, fw_ext))

        if fw_ext != ".hex":
            raise Exception("unknown firmware file type, must be *.hex")

        fw_ihex = IntelHex(fw_fullpath)

        if args.verbose:
            print("fw from 0x%08X to 0x%08X" % (fw_ihex.minaddr(), fw_ihex.maxaddr()))

    if args.bootloader == 'x' and args.firmware == 'x':
        raise Exception("error: no files specified")
    elif args.bootloader != 'x' and args.firmware != 'x':
        if args.replace == False:
            if fw_ihex.minaddr() <= bl_ihex.maxaddr():
                raise Exception("error: firmware overlaps with bootloader")
            bl_ihex.merge(fw_ihex, overlap='ignore')
        else: # replace == True
            fw_binarr = fw_ihex.tobinarray(start = base_address)
            fw_chunk = IntelHex()
            fw_chunk.frombytes(fw_binarr, offset = base_address)
            bl_ihex.merge(fw_chunk, overlap='replace')

        if args.verbose:
            print("merged data from 0x%08X to 0x%08X" % (bl_ihex.minaddr(), bl_ihex.maxaddr()))
    elif args.bootloader == 'x':
        bl_ihex = fw_ihex

    mcu = None
    addr_multi = 1
    if args.mcu is not None and len(args.mcu) > 0:
        mcu = args.mcu.strip().lower()
        if mcu == "f051":
            eep_addr = 0x7C00
            addr_multi = 1
        elif mcu == "g071-64k":
            eep_addr = 0xF800
            addr_multi = 1
        elif mcu == "at421":
            eep_addr = 0xF800
            addr_multi = 1
        elif mcu == "g071-128k":
            eep_addr = 0x1F800
            addr_multi = 4
        else:
            raise Exception("Unknown (or unsupported) MCU specified")

    eep_justname = None
    if mcu is None:
        if "0x" in args.eepromaddr.lower():
            eep_addr = int(args.eepromaddr, 16)
        else:
            try:
                eep_addr = int(args.eepromaddr, 10)
            except:
                eep_addr = int(args.eepromaddr, 16)

    if args.eeprom is not None:
        eep_fullpath = os.path.abspath(args.eeprom)
        eep_basename = os.path.basename(eep_fullpath)
        eep_namesplit = os.path.splitext(eep_basename)
        eep_justname = eep_namesplit[0].strip()
        eep_ext = eep_namesplit[1].strip().lower()
        if args.verbose:
            print("eeprom:")
            print("\t" + eep_fullpath)
            print("\t%s (%s)" % (eep_basename, eep_ext))
        eep_justname = eep_justname.replace(".eepromdump", "")

        if eep_ext != ".bin":
            raise Exception("unknown eeprom file type, must be *.bin")

        eep_ihex = IntelHex()
        eep_ihex.loadbin(eep_fullpath, offset = base_address + eep_addr)
        if args.verbose:
            print("eeprom from 0x%08X to 0x%08X" % (eep_ihex.minaddr(), eep_ihex.maxaddr()))
        if eep_ihex.minaddr() <= bl_ihex.maxaddr():
            raise Exception("error: eeprom overlaps with firmware")
        if args.fresh:
            eep_ihex[base_address + eep_addr + 3] = 0
            eep_ihex[base_address + eep_addr + 4] = 0
            if args.verbose:
                print("eeprom address 3 and 4 erased to appear fresh")
        bl_ihex.merge(eep_ihex, overlap='replace')

    if args.outpath is None:
        outpath = os.path.abspath(".")
    else:
        outpath = args.outpath
    out_abspath = os.path.abspath(outpath)
    if out_abspath.lower().endswith(".hex") == False:
        if os.path.exists(out_abspath):
            if os.path.isdir(out_abspath) == False:
                raise Exception("cannot overwrite %s" % out_abspath)
        else:
            os.makedirs(out_abspath)
        nfname = ""
        if bl_justname is not None and len(bl_justname) > 0:
            nfname += bl_justname
            if fw_justname is not None and len(fw_justname) > 0:
                nfname += "_"
        if fw_justname is not None and len(fw_justname) > 0:
            nfname += fw_justname
        if eep_justname is not None and len(eep_justname) > 0:
            nfname += "_" + eep_justname
        nfname = nfname.replace("-", "_").replace(" ", "_")
        nfname = nfname.strip('_')
        while "_." in nfname or "._" in nfname or ".." in nfname or "__" in nfname:
            nfname = nfname.replace("_.", "_").replace("._", "_").replace("..", ".").replace("__", "_").strip('_')
        out_abspath = os.path.join(out_abspath, nfname + ".hex")

    if bl_ihex.maxaddr() < base_address + eep_addr:
        print("warning: no eeprom data")

    if mcu is None:
        if bl_ihex.maxaddr() >= base_address + (1024 * 64):
            print("warning: data exceeds 64 kb")
        elif bl_ihex.maxaddr() >= base_address + (1024 * 32):
            print("warning: data exceeds 32 kb")
    elif mcu == "f051" and bl_ihex.maxaddr() >= base_address + (1024 * 32):
        print("warning: data exceeds 32 kb")
    elif (mcu == "g071-64k" or mcu == "at421") and bl_ihex.maxaddr() >= base_address + (1024 * 64):
        print("warning: data exceeds 64 kb")
    elif mcu == "g071-128k" and bl_ihex.maxaddr() >= base_address + (1024 * 128):
        print("warning: data exceeds 128 kb")

    if args.singlechunk:
        start_addr = bl_ihex.minaddr()
        end_addr = bl_ihex.maxaddr()
        existing_addresses = set(bl_ihex.addresses())
        print("merging into single chunk, 0x%08X to 0x%08X" % (start_addr, end_addr))
        addr = start_addr
        while addr <= end_addr:
            if addr not in existing_addresses:
                bl_ihex[addr] = 0xFF
            print("\r0x%08X / 0x%08X   " % (addr, end_addr), end="", flush=True)
            addr += 1
        print("\rdone merging           ")

    if args.verbose:
        print("saving to %s" % out_abspath)
    #bl_ihex.write_hex_file(out_abspath)

    def write_hex_file_without_type_05(ih, filename):
        import io
        hex_str = io.StringIO()
        ih.write_hex_file(hex_str)
        hex_str.seek(0)
        # Filter out the type 05 record
        filtered_lines = []
        for line in hex_str:
            if line[7:9] != '05':  # Check the record type
                filtered_lines.append(line)
        # Write the filtered content to the file
        with open(filename, 'w') as f:
            f.writelines(filtered_lines)
    write_hex_file_without_type_05(bl_ihex, out_abspath)

    if args.verbose == False:
        print("saved to %s" % out_abspath)

if __name__ == '__main__':
    main()
