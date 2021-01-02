import os.path as path
import numpy as np
from bgm_util import BaseRecord


class FileIO:

    @staticmethod
    def load_fasta_file(filename):
        """
        Reads pairs of (identifier, sequence) from FASTA file.ipynb
        @param filename:
        @return: generator of pairs of (identifier, sequence)
        """
        if not path.isfile(filename):
            raise FileNotFoundError

        # coverts to uppercase, remove trailing whitespace, and any internal spaces, \r \n
        def seq_from_lines(alllines):
            return "".join(alllines).replace(" ", "").replace("\r", "").replace("\n", "").upper()

        handle = open(filename, 'r')

        # Skip any text before the first record (e.g. blank lines, comments)
        for line in handle:
            if line[0] == ">":
                title = line[1:].rstrip()
                break
        else:
            # no break encountered - probably an empty file
            return

        # Main logic
        lines = []
        for line in handle:
            if line[0] == ">":
                yield BaseRecord(title, seq_from_lines(lines))
                lines = []
                title = line[1:].rstrip()
                continue
            lines.append(line.rstrip())

        handle.close()

        yield BaseRecord(title, seq_from_lines(lines))

    @staticmethod
    def write_silico_maps(maps, filename):
        """
        Writes in siloco maps or sequences
        @param maps: pairs of (identifier, sequence)
        @param filename: filename
        """
        file = open(filename, 'w')
        for map_it in maps:
            # write ID
            file.write('> ' + map_it.name + '\n')
            lines_no = int(len(map_it.sequence) / 80) + (0 if (len(map_it.sequence) % 80) == 0 else 1)
            for i in range(0, lines_no):
                file.write(map_it.sequence[i * 80:(i + 1) * 80] + '\n')
        file.close()

    @staticmethod
    def write_valouev_maps(maps, filename):
        """
        Writes valouev style maps file
        @param maps: pairs of (identifier, sequence)
        @param filename: filename
        """
        # unit = Kbp

        # "maps" - tables [name, [distance, distance, ...]]
        file = open(filename, 'w')
        index = 0
        for map_it in maps:
            # "first line" write ID ?? no_of_dists
            file.write(str(index) + "name" + str(len(map_it[1])) + '\n')

            # "second line" enzyme name & acronym followed by space separated maps
            file.write("enzyme_name" + " " + "enzyme_acronym" + " ")

            # write distances
            dists = ""
            for i in map_it[1]:
                dists = dists + " " + str(i / 1000)
            file.write(dists + "\n")

            # "third line" -> empty
            file.write("\n")

            index = index + 1
        file.close()

    @staticmethod
    def parse_BNX_ref_file(filename):
        """
        Reads BNX file with one record
        @param filename: filename
        @return: tuple with (distances as array, Recognition Site)
        """
        if not path.isfile(filename):
            raise FileNotFoundError

        infile = open(filename)
        distances = []
        site = None
        for line in infile:
            if line.startswith('1'):
                distances = np.array(line[2:].rstrip('\n').split('\t'))
                distances = distances.astype(np.float)
            # elif line.startswith('0'):
            #    # metadata
            elif line.startswith('#'):
                if line.startswith('# Nickase Recognition Site'):
                    site = line.rstrip('\t').split(':')[1]
            # elif line.startswith('Q'):
            #     # Quality data line QX11 or QX12
        infile.close()

        return distances, site

    @staticmethod
    def parse_BNX_file(filename):
        """
        Reads BNX file with multiple rmaps
        @param filename: filename
        @return: tuple with list of distances as array
        """
        if not path.isfile(filename):
            raise FileNotFoundError

        infile = open(filename)
        distances = []
        for line in infile:
            if line.startswith('1'):
                line_dist = np.array(line[2:].rstrip('\n').split('\t'))
                distances.append(line_dist.astype(np.float))
            # elif line.startswith('0'):
            #    # metadata
            # elif line.startswith('Q'):
            #     # Quality data line QX11 or QX12

        return distances

    @staticmethod
    def parse_CMAP_file(filename):
        """
        Reads CMAP file with optical map
        @param filename: filename
        @return: list of distances as array
        """
        if not path.isfile(filename):
            raise FileNotFoundError(filename)

        infile = open(filename)
        distances = []
        for line in infile:
            #if line.startswith('1'):
            if not line.startswith('#'):
                line_vals = np.array(line.rstrip('\n').split('\t'))
                distances.append(int(line_vals[5]))


        return distances
