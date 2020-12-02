import numpy as np


class DnaUtil:
    """
    Util for DNA seguence string generation and manipulation
    """
    bases = ['A', 'C', 'G', 'T']
    base_complements = {'A': 'T', 'C': 'G', 'G': 'C', 'T': 'A'}

    @classmethod
    def random_genome(cls, length=10, seed=0):
        """
        Creates random genome of given length
        @param length: length of desired genome
        @param seed: seed for random number generator
        @return: random genome string
        """
        if seed != 0:
            np.random.seed(seed)

        rand_gen = []
        for _ in range(length):
            rand_gen.append(
                cls.bases[np.random.randint(0, len(cls.bases))]
            )

        return ''.join(rand_gen)

    @classmethod
    def complement_list(cls, seq):
        """
        Creates complementary DNA sequence list
        @param seq: DNA sequence
        @return: list of bases
        """
        bases_seq = list(seq)
        return [cls.base_complements.get(base, base) for base in bases_seq]

    @classmethod
    def complement(cls, seq):
        """
        Creates complementary DNA sequence string
        @param seq: DNA sequence
        @return: complementary DNA string
        """
        return ''.join(cls.complement_list(seq))

    @classmethod
    def reverse_complement(cls, seq):
        """
        Creates reverse complementary DNA sequence string
        @param seq: DNA sequence
        @return: reversed complementary DNA string
        """
        return ''.join(reversed(cls.complement_list(seq)))
