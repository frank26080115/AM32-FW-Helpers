import argparse
import os
from intelhex import IntelHex

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("bootloader", type=str, help="bootloader file")
    parser.add_argument("firmware", type=str, help="firmware file")
    parser.add_argument("-a", "--baseaddr", metavar="baseaddr", default="0x08000000", type=str, help="base address")
    parser.add_argument("-e", "--eeprom", metavar="eeprom", default=None, type=str, help="eeprom file")
    parser.add_argument("-x", "--eepromaddr", metavar="eepromaddr", default="0x7C00", type=str, help="eeprom address")
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

    if fw_ihex.minaddr() - 1 <= bl_ihex.maxaddr():
        raise Exception("error: firmware overlaps with bootloader")

    bl_ihex.merge(fw_ihex, overlap='ignore')

    if args.verbose:
        print("merged data from 0x%08X to 0x%08X" % (bl_ihex.minaddr(), bl_ihex.maxaddr()))

    eep_justname = None
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
        if eep_ihex.minaddr() - 1 <= bl_ihex.maxaddr():
            raise Exception("error: eeprom overlaps with firmware")
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
        nfname = bl_justname + "_" + fw_justname
        if eep_justname is not None and len(eep_justname) > 0:
            nfname += "_" + eep_justname
        nfname = nfname.replace("-", "_").replace(" ", "_")
        while "_." in nfname or "._" in nfname or ".." in nfname or "__" in nfname:
            nfname = nfname.replace("_.", "_").replace("._", "_").replace("..", ".").replace("__", "_")
        out_abspath = os.path.join(out_abspath, nfname + ".hex")

    if bl_ihex.maxaddr() < base_address + eep_addr:
        print("warning: no eeprom data")

    if bl_ihex.maxaddr() >= base_address + (1024 * 64):
        print("warning: data exceeds 64 kb")
    elif bl_ihex.maxaddr() >= base_address + (1024 * 32):
        print("warning: data exceeds 32 kb")

    if args.verbose:
        print("saving to %s" % out_abspath)
    bl_ihex.write_hex_file(out_abspath)
    if args.verbose == False:
        print("saved to %s" % out_abspath)

if __name__ == '__main__':
    main()