import os.path as path
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
