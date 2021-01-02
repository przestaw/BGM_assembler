import argparse
from bgm_util import *
import os

genome_files = {'e.coli': '../fasta/E.Coli/GCF_000005845.2_ASM584v2_genomic.fna',
                'yeast': '../fasta/Yeast/GCF_000146045.2_R64_genomic.fna',
                'roundworm': '../fasta/Roundworm/GCF_000002985.6_WBcel235_genomic.fna',
                'rice': '../fasta/Rice/GCA_009829725.1_ASM982972v1_genomic.fna'
                }

parser = argparse.ArgumentParser(description='Simulate digestion of genome - by Stawczyk Przemyslaw')

parser.add_argument('-e', '--enzymes',
                    help='Define enzymes to use',
                    nargs='+',
                    type=str,
                    required=False,
                    default=['BspQI', 'BssSI'])
parser.add_argument('-c', '--coverages',
                    help='Define how many times genome should be covered.',
                    nargs='+',
                    type=float,
                    required=False,
                    default=[5, 7.5, 10, 15, 25])
parser.add_argument('-g', '--genomes',
                    help='Define genomes to use [from predefined]',
                    nargs='+',
                    type=str,
                    required=False,
                    default=['e.coli'])
parser.add_argument('-s', '--seed',
                    help='Define seed for generation',
                    required=False,
                    default=4231)
parser.add_argument('-q', '--quant',
                    help='Define quant size',
                    required=False,
                    default=1000)
parser.add_argument('-p', '--path',
                    help='Define path for output',
                    required=False,
                    default="")

if __name__ == '__main__':
    args = parser.parse_args()

    # load
    genomes = {}
    for i in args.genomes:
        genomes[i] = [DNARecord.from_BaseRecord(j) for j in FileIO.load_fasta_file(genome_files[i])]

    # digest
    digested = {}
    for enzyme in args.enzymes:
        digested[enzyme] = {}
        # digest with each enzyme
        for species, genome in genomes.items():
            digested[enzyme][species] = []
            for record in genome:
                digested[enzyme][species].append(record.digest_with_enzyme(enzyme))

    m_path = args.path
    # generate maps
    for enzyme in args.enzymes:
        for species, genome in digested[enzyme].items():
            path = m_path + "compare/" + species + "/" + enzyme + "/"
            try:
                os.makedirs(path)
            except FileExistsError:
                print("path exists")

            for coverage in args.coverages:
                maps = Generator.generate_compare_maps_dig(genome, quant=int(args.quant), coverage=coverage)
                # write each generated map
                os.makedirs(path + str(coverage) + "/")
                FileIO.write_valouev_maps(maps[1], path + str(coverage) + "/" + species +
                                          '-enz-' + enzyme +
                                          '-cov-' + str(coverage) + '.valouev')

                FileIO.write_silico_maps(maps[0], path + str(coverage) + "/" + species +
                                         '-enz-' + enzyme +
                                         '-cov-' + str(coverage) +
                                         '-q-' + str(args.quant) + '.binmaps')
