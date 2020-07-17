import numpy as np

def generate_errorfree_overlaping(genome, one_map_len, shift=100):
    """
    Generates set of maps with same size
    :param genome: pair of [genome name][optical map]
    :param one_map_len: length of one map
    :param shift: length of shift between two neighbour maps
    :return: set of rmaps
    """
    i = 0
    j = one_map_len

    example = []
    while j < len(genome[1]):
        temp = [str(i) + '_' + str(j) + ' ' + genome[0], genome[1][i:j]]
        i = i + shift
        j = j + shift
        example.append(temp)

    return example


def generate_errorfree_distribued(genome, one_map_len, std_dev, coverage=15):
    """
    Generates set of maps with size from normal distribution
    :param genome: pair of [genome name][optical map]
    :param one_map_len: avg length of one map
    :param std_dev: std dev of map length
    :param coverage: avg coverage of genome by maps
    :return:
    """
    length = np.random.normal(one_map_len, std_dev)
    raise NotImplemented
