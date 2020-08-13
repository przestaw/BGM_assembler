import argparse

import digest_util as dig
import dna_util as dna
import files_util as files
import script_functions as fun

types_of_maps = ['perfect_overlap', 'normal', 'valid_normal']

parser = argparse.ArgumentParser(description='Simulate digestion of genome - by Stawczyk Przemyslaw')

parser.add_argument('-c', '--coverage', help='Define how many maps you need - for normally distributed maps',
                    required=False, default=15)
parser.add_argument('-s', '--shift', help='Define shift between maps - for perfect overlap', required=False,
                    default=100)
parser.add_argument('-q', '--quant', help='Define how long quant you need (in bases)', required=False,
                    default=2000)
parser.add_argument('-l', '--length', help='Define how long maps you need (in bases)', required=False,
                    default=750)
parser.add_argument('-d', '--std_dev', help='Define standard deviation for map len - for normally distributed maps',
                    required=False,
                    default=50)
parser.add_argument('-e', '--enzyme', help='Choose enzyme for digest : ' + str(
        [enzyme + '(' + pattern + ')' for enzyme, pattern in dig.default_enzymes.items()]), required=False,
                    default='BspQI')
parser.add_argument('-i', '--input', help='Define seq source', required=False, default='rand')
parser.add_argument('-t', '--type', help='Define type: ' + str(types_of_maps), required=False, default='normal')
parser.add_argument('-f', '--file', help='FASTA genome input file', required=False)
parser.add_argument('-o', '--output', help='Define seq source', required=False, default='out.omap')

if __name__ == '__main__':
    args = parser.parse_args()

    sequences = []

    if args.input == 'rand':
        sequences = [['rand_gen_len_30_Mb', dna.random_genome(lenght=30 * 1000000, seed=4782)]]
    else:
        if args.file:
            sequences = [i for i in files.load_fasta_file(args.file)]

    result = None

    if args.enzyme not in dig.default_enzymes.keys():
        raise Exception('No such enzyme', args.enzyme)

    if args.type == 'perfect_overlap':
        result = fun.generate_maps_shifted_from_seq(
                sequences=sequences, enzyme=args.enzyme, quant=args.quant, length=args.legth, shift=args.shift)
    elif args.type == 'normal':
        result = fun.generate_maps_distributed_from_seq(
                sequences=sequences, enzyme=args.enzyme, quant=args.quant,
                coverage=args.coverage, length=args.legth, std_dev=args.std_dev)
    elif args.type == 'valid_normal':
        result = fun.generate_valid_maps_from_seq(
                sequences=sequences, enzyme=args.enzyme, quant=args.quant, coverage=args.coverage)
    else:
        raise Exception('No such mode', args.type)

    files.write_silico_maps(result, args.output)
