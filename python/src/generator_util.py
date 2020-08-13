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
        temp = [str(i) + ' ' + str(j) + ' |' + genome[0], genome[1][i:j]]
        i = i + shift
        j = j + shift
        example.append(temp)

    return example


def generate_errorfree_distribued(genome, one_map_len, std_dev, coverage=25, seed=4231):
    """
    Generates set of maps with size from normal distribution
    :param genome: pair of [genome name][optical map]
    :param one_map_len: avg length of one map
    :param std_dev: std dev of map length
    :param coverage: avg coverage of genome by maps
    :param seed: seed for numpy random module
    :return: set of rmaps
    """
    # set seed for random module
    np.random.seed(seed)
    example = []
    target = len(genome[1]) * coverage
    so_far = 0

    while so_far < target:
        length = np.random.normal(one_map_len, std_dev)
        length = int(abs(length))

        start = np.random.random_integers(0, len(genome[1]) - length)
        end = int(start + length)

        temp = [str(start) + ' ' + str(end) + ' |' + genome[0], genome[1][start:end]]

        so_far = so_far + length
        example.append(temp)

    return example


def generate_errorfree_distribued_start_end_on_1(genome, one_map_len, std_dev, coverage=25, seed=4231):
    """
    Generates set of maps with size from normal distribution
    :param genome: pair of [genome name][optical map]
    :param one_map_len: avg length of one map
    :param std_dev: std dev of map length
    :param coverage: avg coverage of genome by maps
    :param seed: seed for numpy random module
    :return: set of rmaps
    """
    # set seed for random module
    np.random.seed(seed)
    example = []
    target = len(genome[1]) * coverage
    so_far = 0

    while so_far < target:
        length = np.random.normal(one_map_len, std_dev)
        length = int(abs(length))

        start = np.random.random_integers(0, len(genome[1]) - length)
        end = int(start + length)

        # refine start index
        while genome[1][start] == '0':
            start = start + 1

        # refine end index
        while genome[1][end] == '0':
            end = end - 1

        # check lenght, if negative = skip
        length = end - start
        if length < 0:
            continue

        temp = [str(start) + ' ' + str(end) + ' |' + genome[0], genome[1][start:end]]

        so_far = so_far + length
        example.append(temp)

    return example
