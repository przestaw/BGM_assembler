import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Simulate digestion of genome - by PrzeStaw')

    parser.add_argument('-c', '--count', help='Define how many maps you need', required=False, default=15)
    parser.add_argument('-l', '--length', help='Define how long maps you need (in kilo-bases)', required=False, default=64)
    parser.add_argument('-i', '--input', help='Define seq source', required=False, default='rand')
    parser.add_argument('-f', '--file', help='FASTA genome input file', required=False)
    parser.add_argument('-o', '--output', help='Define seq source', required=False, default='out.omap')

    args = parser.parse_args()

    # TODO - load and output

