import numpy as np
import copy
import os
from bgm_util import BinaryRecord, DigestUtil
from multiprocessing import Manager, Pool


class Generator:
    """
    Class for generation of binary maps from sequences and maps
    """

    @staticmethod
    def generate_refmap_and_reversed_from_dig(digest, quant):
        """
        Generates reference binary map from digest
        @param digest: digest
        @param quant: quant size
        @return: binary reference map and reversed map
        """
        forward_map = copy.deepcopy(digest)

        result = []
        for it in forward_map:
            it.remove_0_ends()
            it.quantize(quant)

        for rev_it in forward_map:
            reverse_map = BinaryRecord('reverse ' + rev_it.name, rev_it.sequence[::-1])
            result.append(reverse_map)

        result.extend(forward_map)

        return result

    @classmethod
    def generate_refmap_and_reversed_from_seq(cls, sequence, enzyme, quant):
        """
        Generates reference binary map from genomic sequence
        @param sequence: genomic sequence
        @param enzyme: enzyme
        @param quant: quant size
        @return: binary reference map and reversed map
        """
        digest = []
        for seq in sequence:
            digest.append(seq.digest_with_enzyme(enzyme=enzyme))
        return cls.generate_refmap_and_reversed_from_dig(digest, quant)

    @staticmethod
    def digest_and_quantize_seq(sequences, enzyme, quant):
        """
        Digest and quantize set of sequences
        @param sequences: sequences
        @param enzyme: enzyme name
        @param quant: quant size
        @return: maps
        """
        digest = []
        for idx, seq_it in enumerate(sequences, start=1):
            tmp = seq_it.digest_with_enzyme(enzyme=enzyme)
            tmp.name = 'chr ' + str(idx) + ' ' + seq_it.name
            digest.append(tmp)

        maps = []
        for dig_it in digest:
            maps.append(dig_it.gen_bin_map_from_digest(quantize=int(quant)))

        return maps

    @staticmethod
    def generate_errorfree_distribued_start_end_1_simple(genome, one_map_len, std_dev, coverage=25, seed=4231):
        """
        Generates set of maps with size from normal distribution
        @param genome: pair of [genome name][optical map]
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param coverage: avg coverage of genome by maps
        @param seed: seed for numpy random module
        @return: set of rmaps
        """
        # set seed for random module
        np.random.seed(seed)
        example = []
        target = len(genome.sequence) * coverage
        so_far = 0

        idx = 1

        while so_far < target:
            length = np.random.normal(one_map_len, std_dev)
            length = int(abs(length))
            center = np.random.random_integers(0, len(genome.sequence))

            start = int(center - length / 2)
            end = int(center + length / 2)

            if start < 0:
                end = end - start
                start = 0

            if end >= len(genome.sequence):
                diff = 1 + end - len(genome.sequence)
                end = len(genome.sequence) - 1
                # state when start:end is whole genome should NOT happen but must be handled
                start = start - diff if start - diff >= 0 else 0

            temp = BinaryRecord.from_BaseRecord(genome[start:end])
            temp.remove_0_ends()

            if len(temp) < (one_map_len - 3 * std_dev):
                continue

            temp.name = str(start) + " " + str(end) + " - " + str(idx) + ' map from : ' + genome.name
            idx = idx + 1

            so_far = so_far + length
            example.append(temp)

        return example

    @classmethod
    def generate_valid_maps_from_seq(cls, sequences, enzyme, quant, one_map_len=1000000, std_dev=300000, coverage=25,
                                     seed=4231):
        """
        Generates normally distribute maps, starting&ending on labels
        @param sequences: genomic sequences
        @param enzyme: enzyme name
        @param quant: quant size
        @param coverage: coverage
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param seed: seed
        @return: set of rmaps
        """
        # digest with enzyme
        for idx, seq_it in enumerate(sequences, start=1):
            seq_it.digest_with_enzyme(enzyme=enzyme)
            seq_it.name = 'chr ' + str(idx) + ' ' + seq_it.name

        return cls.generate_valid_maps_from_dig(sequences, quant, coverage, one_map_len, std_dev, seed)

    @classmethod
    def generate_valid_maps_from_dig(cls, digests, quant, coverage=25, one_map_len=1000000, std_dev=300000,
                                     seed=4231, processes=0):
        """
        Generates normally distribute maps, starting&ending on labels
        @param digests: genomic sequences digest
        @param quant: quant size
        @param coverage: coverage
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param seed: seed
        @param processes: no of processes, if 0 passed it will fallback to os.cpu_count() - 1
        @return: set of rmaps
        """

        # run multiprocess
        man = Manager()
        lock = man.Lock()
        maps = man.list()

        if processes == 0:
            processes = os.cpu_count() - 1

        global runner  # workaround for pool

        def runner(genome):
            # generate maps
            result = cls.generate_errorfree_distribued_start_end_1_simple(
                genome, one_map_len, std_dev, coverage=coverage, seed=seed)

            # quantize
            seq = []
            for dig_iter in result:
                seq.append(dig_iter.quantize(quant=quant))

            # output result
            with lock:
                maps.extend(seq)

        pool = Pool(processes=processes)
        with pool:
            pool.map(func=runner,
                     iterable=[gen_iter for gen_iter in digests])

        return maps

    @staticmethod
    def generate_errorfree_distribued(genome, one_map_len, std_dev, coverage=25, seed=4231):
        """
        Generates set of maps with size from normal distribution
        @param genome: pair of [genome name][optical map]
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param coverage: avg coverage of genome by maps
        @param seed: seed for numpy random module
        @return: set of rmaps
        """
        # set seed for random module
        np.random.seed(seed)
        example = []
        target = len(genome.sequence) * coverage
        so_far = 0

        while so_far < target:
            length = np.random.normal(one_map_len, std_dev)
            length = int(abs(length))

            start = np.random.random_integers(0, len(genome.sequence) - length)
            end = int(start + length)

            temp = BinaryRecord.from_BaseRecord(genome[start:end])
            temp.name = str(start) + ' ' + str(end) + ' |' + genome.name

            so_far = so_far + length
            example.append(temp)

        return example

    @classmethod
    def generate_maps_distributed_from_seq(cls, sequences, enzyme, quant, length, std_dev, coverage, seed=4231):
        """
        Generates normally distribute maps, starting&ending on qunats of refmap
        @param sequences: genomic sequences
        @param enzyme: enzyme name
        @param quant: quant size
        @param length: expected avg length
        @param std_dev: std deviation of normal distribution
        @param coverage: coverage
        @param seed: seed
        @return: set of rmaps
        """
        maps = cls.digest_and_quantize_seq(sequences, enzyme, quant)

        result = []
        for i in maps:
            result.extend(
                cls.generate_errorfree_distribued(i, one_map_len=int(length), std_dev=int(std_dev), coverage=coverage))

        return result

    @staticmethod
    def generate_errorfree_shifted(genome, one_map_len, shift=100):
        """
        Generates set of maps with same size
        @param genome: pair of [genome name][optical map]
        @param one_map_len: length of one map
        @param shift: length of shift between two neighbour maps
        @return: set of rmaps
        """
        i = 0
        j = one_map_len

        example = []
        while j < len(genome.sequence):
            temp = genome[i:j]
            temp.name = str(i) + ' ' + str(j) + ' |' + genome.name
            i = i + shift
            j = j + shift
            example.append(temp)

        return example

    @classmethod
    def generate_maps_shifted_from_seq(cls, sequences, enzyme, quant, length, shift):
        """
        Generates shifted overlapping maps, starting&ending on qunats of refmap
        @param sequences: genomic sequences
        @param enzyme: enzyme name
        @param quant: quant size
        @param length: length
        @param shift: shift
        @return: set of rmaps
        """
        maps = cls.digest_and_quantize_seq(sequences, enzyme, quant)

        result = []
        for i in maps:
            result.extend(cls.generate_errorfree_shifted(i, one_map_len=int(length), shift=int(shift)))

        return result

    @classmethod
    def generate_valid_distances_maps(cls, digests, coverage=25, one_map_len=1000000, std_dev=300000, seed=4231):
        """
        Generates normally distribute maps, starting&ending on labels
        @param digests: genomic sequences digest
        @param quant: quant size
        @param coverage: coverage
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param seed: seed
        @return: set of rmaps
        """

        # sample genome strands
        seq = []
        for gen_iter in digests:
            seq.extend(cls.generate_errorfree_distribued_start_end_1_simple(
                gen_iter, one_map_len, std_dev, coverage=coverage, seed=seed))

        maps = []
        for dig_iter in seq:
            maps.append([dig_iter.name, DigestUtil.gen_distances_from_quants(dig_iter.sequence)])

        return maps

    @classmethod
    def generate_compare_maps_dig(cls, digests, quant, coverage=25, one_map_len=1000000, std_dev=300000,
                                     seed=4231, processes=0):
        """
        Generates normally distribute maps, starting&ending on labels
        @param digests: genomic sequences digest
        @param quant: quant size
        @param coverage: coverage
        @param one_map_len: avg length of one map
        @param std_dev: std dev of map length
        @param seed: seed
        @param processes: no of processes, if 0 passed it will fallback to os.cpu_count() - 1
        @return: set of rmaps
        """

        # run multiprocess
        man = Manager()
        lock = man.Lock()
        maps = man.list()
        valouev = man.list()

        if processes == 0:
            processes = os.cpu_count() - 1

        global runner  # workaround for pool

        def runner(genome):
            # generate maps
            result = cls.generate_errorfree_distribued_start_end_1_simple(
                genome, one_map_len, std_dev, coverage=coverage, seed=seed)

            # quantize and valouevize
            seq = []
            val = []
            for dig_iter in result:
                val.append([dig_iter.name, DigestUtil.gen_distances_from_quants(dig_iter.sequence)])
                seq.append(dig_iter.quantize(quant=quant))

            # output result
            with lock:
                maps.extend(seq)
                valouev.extend(val)

        pool = Pool(processes=processes)
        with pool:
            pool.map(func=runner,
                     iterable=[gen_iter for gen_iter in digests])

        return [maps, valouev]
