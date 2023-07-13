import argparse
import os
from intelhex import IntelHex

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("filepath", type=str, help="input dumped file path")
    parser.add_argument("-a", "--baseaddr", metavar="baseaddr", default="0x08000000", type=str, help="base address")
    parser.add_argument("-x", "--eepromaddr", metavar="eepromaddr", default="0x7C00", type=str, help="eeprom address")
    parser.add_argument("-y", "--eepromlen", metavar="eepromlen", default="176", type=str, help="eeprom length")
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

    if "0x" in args.eepromaddr.lower():
        eep_addr = int(args.eepromaddr, 16)
    else:
        try:
            eep_addr = int(args.eepromaddr, 10)
        except:
            eep_addr = int(args.eepromaddr, 16)

    if "0x" in args.eepromlen.lower():
        eep_len = int(args.eepromlen, 16)
    else:
        try:
            eep_len = int(args.eepromlen, 10)
        except:
            eep_len = int(args.eepromlen, 16)


    eep_addr = base_address + eep_addr

    if args.verbose:
        print("base addr 0x%08X , eeprom addr 0x%08X (len %u)" % (base_address, eep_addr, eep_len))

    fw_fullpath = os.path.abspath(args.filepath)
    fw_basename = os.path.basename(fw_fullpath)
    fw_namesplit = os.path.splitext(fw_basename)
    fw_justname = fw_namesplit[0].strip()
    fw_ext = fw_namesplit[1].strip().lower()
    if args.verbose:
        print("input file path:")
        print("\t" + fw_fullpath)
        print("\t%s (%s)" % (fw_basename, fw_ext))

    if fw_ext == ".bin":
        fw_ihex = IntelHex()
        fw_ihex.loadbin(fw_fullpath, offset = base_address)
    elif fw_ext == ".hex":
        fw_ihex = IntelHex(fw_fullpath)
    else:
        raise Exception("unknown file type, must be *.bin or *.hex")

    fw_ihex.padding = 0xFF

    if args.verbose:
        print("file read from 0x%08X to 0x%08X" % (fw_ihex.minaddr(), fw_ihex.maxaddr()))

    if fw_ihex.maxaddr() < eep_addr:
        raise Exception("file does not contain eeprom data")
        #print("warning: file does not contain eeprom data")

    j = None
    i = fw_ihex.maxaddr()
    while i >= fw_ihex.minaddr() and j is None:
        if fw_ihex[i] != 0xFF:
            j = i
            break
        i -= 1
    if j is not None:
        j += 8
        j = min(fw_ihex.maxaddr(), j)
        if j - eep_addr < eep_len:
            j = eep_addr + eep_len + 8
        j = min(fw_ihex.maxaddr(), j)
        fw_ihex = fw_ihex[:j]
        if args.verbose:
            print("file trimmed to [0x%08X : 0x%08X]" % (fw_ihex.minaddr(), fw_ihex.maxaddr()))

    if args.outpath is None:
        outpath = os.path.abspath(".")
    else:
        outpath = args.outpath
    out_abspath = os.path.abspath(outpath)
    if out_abspath.lower().endswith(".bin") == False:
        if os.path.exists(out_abspath):
            if os.path.isdir(out_abspath) == False:
                raise Exception("cannot overwrite %s" % out_abspath)
        else:
            os.makedirs(out_abspath)
        nfname = fw_justname + ".eepromdump"
        nfname = nfname.replace("-", "_").replace(" ", "_")
        while "_." in nfname or "._" in nfname or ".." in nfname or "__" in nfname:
            nfname = nfname.replace("_.", "_").replace("._", "_").replace("..", ".").replace("__", "_")
        out_abspath = os.path.join(out_abspath, nfname)

    if args.verbose:
        print("saving to %s.***" % out_abspath)

    fw_ihex.tobinfile(out_abspath + ".bin", start = eep_addr)
    fw_ihex = fw_ihex[eep_addr:]
    fw_ihex.write_hex_file(out_abspath + ".hex")

    if args.verbose == False:
        print("saved to %s.***" % out_abspath)

if __name__ == '__main__':
    main()
