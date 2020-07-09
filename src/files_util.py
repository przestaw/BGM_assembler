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


def write_silico_maps(maps):
    # TODO : maps - table of pairs ?
    # TODO : format ?
    raise NotImplemented
