import numpy as np

bases = ['A', 'C', 'G', 'T']
base_complements = {'A': 'T', 'C': 'G', 'G': 'C', 'T': 'A'}


def random_genome(lenght=10, seed=0):
    if seed != 0:
        np.random.seed(seed)

    rand_gen = []
    for _ in range(lenght):
        rand_gen.append(
                bases[np.random.randint(0, len(bases))]
        )

    return ''.join(rand_gen)


def complement_list(seq):
    bases_seq = list(seq)
    return [base_complements.get(base, base) for base in bases_seq]


def complement(seq):
    return ''.join(complement_list(seq))


def reverse_complement(seq):
    return ''.join(reversed(complement_list(seq)))
