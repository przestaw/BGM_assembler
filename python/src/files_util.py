import os.path as path


# returns pairs of (identifier, sequence) from FASTA file
def load_fasta_file(filename):
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
            yield title, seq_from_lines(lines)
            lines = []
            title = line[1:].rstrip()
            continue
        lines.append(line.rstrip())

    handle.close()

    yield title, seq_from_lines(lines)


# maps - table of pairs [ID, sequence]
def write_silico_maps(maps, filename):
    file = open(filename, 'w')
    for map_it in maps:
        # write ID
        file.write('> '+map_it[0]+'\n')
        lines_no = int(len(map_it[1])/80) + (0 if (len(map_it[1]) % 80) == 0 else 1)
        for i in range(0, lines_no):
            file.write(map_it[1][i*80:(i+1)*80]+'\n')
    file.close()
