from bgm_util import DnaUtil

# Nt.BspQI is a nicking endonuclease that cleaves only one strand of DNA on a double-stranded DNA substrate.
# 5' ... G C T C T T C N^ ... 3'
# 3' ... C G A G A A G M  ... 5'
# BssSI restriction enzyme recognizes CACGAG - Nb.BssSI cuts one site
# 5' ... C^A C G A G ... 3'
# 3' ... G T G C T^C ... 5'
# Nt.BbvCI
# 5' ... C C^T C A G C... 3'
# 3' ... G G A G T C G... 5'
# Nb.BsrDI
# 5' ... G C A A T G N N ... 3'
# 3' ... C G T T A C^N N ... 5'


class DigestUtil:
    """
    Class designated for restriction enzyme handling
    """
    default_enzymes = {'BspQI': 'GCTCTTC', 'BssSI': 'CACGAG', 'BbvCI': 'CCTCAGC', 'BsrDI': 'GCAATG'}

    @classmethod
    def digest_with_enzyme(cls, genome, enzyme):
        """
        Digests genome string with base resolution - marks first occurance at position [using sliding window approach]
        @param genome: genomic sequence string
        @param enzyme: enzyme name
        @return: digest of genome len, 1 indicates recognized pattern
        """
        if enzyme in cls.default_enzymes.keys():
            return cls.digest_with_pattern(genome, cls.default_enzymes[enzyme])
        else:
            raise Exception('No such enzyme', enzyme)

    @staticmethod
    def digest_with_pattern(genome, enzyme_pattern):
        """
        Digests genome string with base resolution - marks first occurance at position [using sliding window approach]
        @param genome: genomic sequence string
        @param enzyme_pattern: enzyme recognition pattern
        @return: digest of genome len, 1 indicates recognized pattern
        """
        genome_len = len(genome)
        pattern_len = len(enzyme_pattern)

        # prepare patterns
        patterns = [enzyme_pattern]
        if DnaUtil.reverse_complement(enzyme_pattern) != enzyme_pattern:
            patterns.append(DnaUtil.reverse_complement(enzyme_pattern))

        # rev_pat = []
        # for p in patterns:
        #     rev = ''.join(reversed(p))
        #     if p != rev:
        #         rev_pat.append(rev)
        #
        # patterns.extend(rev_pat)

        # main logic
        result = []
        for index in range(0, genome_len - pattern_len):
            if genome[index:(index + pattern_len)] in patterns:
                result.append('1')
            else:
                result.append('0')

        # last positions
        for _ in range(pattern_len):
            result.append('0')

        return ''.join(result)

    @staticmethod
    def gen_bin_map_from_digest(digest, quantize=1000):
        """
        quantize digestion 1 in quant if at least one digestion place was found
        @param digest: digested string
        @param quantize: quantice size
        @return: quanted binary map from digest
        """
        quants_no = int(len(digest) / quantize) + (0 if (len(digest) % quantize) == 0 else 1)

        result = []
        for quant in range(0, quants_no):
            flag = '0'
            # last digest might be smaller - slicing takes care of that
            for i in digest[quant * quantize:(quant + 1) * quantize]:
                if i == '1':
                    flag = '1'
                    break
            result.append(flag)

        return ''.join(result)

    @staticmethod
    def gen_distances_from_quants(digest, symbol='1'):
        """
        Counts distances between labels
        @param digest: digested string
        @param symbol: detected symbol
        @return: list of distances of symbol in digest
        """
        dist = 1
        map = []

        for i in digest:
            if i == symbol:
                map.append(dist)
                dist = 1
            else:
                dist = dist + 1

        return map

    @staticmethod
    def gen_positions_from_digest(digest, symbol='1'):
        """
        Counts positions between labels
        @param digest: digested string
        @param symbol: detected symbol
        @return: list of position of symbol in digest
        """
        dist = 0
        rmap = []

        for i in digest:
            if i == symbol:
                rmap.append(dist)
            dist = dist + 1

        return rmap

    @staticmethod
    def remove_overhang(seq, symbol='0'):
        """
        Removes 0 from end and beginning of seq
        @param seq: sequence
        @param symbol: symbol to be removed from ends
        @return: modified sequence
        """
        ret = seq.lstrip(symbol)
        ret = ret.rstrip(symbol)
        return ret
