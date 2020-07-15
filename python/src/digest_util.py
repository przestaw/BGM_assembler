from src.dna_util import reverse_complement

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

default_enzymes = {'BspQI': 'GCTCTTC', 'BssSI': 'CACGAG', 'BbvCI': 'CCTCAGC', 'BsrDI': 'GCAATG'}


def digest_with_enzyme(genome, enzyme):
    if enzyme in default_enzymes.keys():
        return digest_with_pattern(genome, default_enzymes[enzyme])
    else:
        raise Exception('No such enzyme', enzyme)


# base resolution - marks first occurance at position [using sliding window approach]
def digest_with_pattern(genome, enzyme_pattern):
    genome_len = len(genome)
    pattern_len = len(enzyme_pattern)

    # prepare patterns
    patterns = [enzyme_pattern]
    if reverse_complement(enzyme_pattern) != enzyme_pattern:
        patterns.append(reverse_complement(enzyme_pattern))

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


# quantize digestion 1 in quant if at least one digestion place was found
def gen_bin_map_from_digest(digest, quantize=1000):
    quants_no = int(len(digest) / quantize) + (0 if (len(digest) % quantize) == 0 else 1)

    result = []
    for quant in range(0, quants_no):
        flag = '0'
        # last digest might be smaller - slicing takes care of that
        for i in digest[quant * quantize:(quant+1) * quantize]:
            if i == '1':
                flag = '1'
                break
        result.append(flag)

    return ''.join(result)


# count distances between labels
def gen_distances_from_quants(quanted, symbol = '1'):
    dist = 1
    map = []

    for i in quanted:
        if i == symbol:
            map.append(dist)
            dist = 1
        else:
            dist = dist + 1

    return map


# count distances between labels
def gen_positions_from_digest(digest, symbol = '1'):
    dist = 0
    map = []

    for i in digest:
        if i == symbol:
            map.append(dist)
        dist = dist + 1

    return map
