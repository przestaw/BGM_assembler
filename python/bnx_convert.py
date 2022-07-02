"""
Command-line utility converting BioNano Genomics BNX files into quantized FASTA01 binary map files.
"""
from bgm_util import bnx_util, files_util
import argparse, os

parser = argparse.ArgumentParser(
    description="BNX to Fasta01 - by Stawczyk Przemyslaw"
)

parser.add_argument('-q', '--quants',
                    help='Define how long quant you need (in bases)',
                    required=False,
                    nargs='+',
                    type=int,
                    default=[1000, 1500])
parser.add_argument('-f', '--file',
                    help='BNX genome input file',
                    required=True)
parser.add_argument('-l', '--length',
                    help='Min map length in bases',
                    required=False,
                    default=750000)
parser.add_argument('-p', '--path',
                    help='Define path for output',
                    required=False,
                    default="./")

if __name__ == "__main__":
    args = parser.parse_args()

    file = os.path.basename(args.file)

    for q in args.quants:
        filename = os.path.join(args.path, file + ".q" + str(q) + ".fasta01")
        maps = bnx_util.create_binary_maps(args.file, quant=q)
        min_len_q = args.length / q
        maps = [m for m in maps if len(m) >= min_len_q]
        files_util.FileIO.write_silico_maps(maps=maps, filename=filename)
