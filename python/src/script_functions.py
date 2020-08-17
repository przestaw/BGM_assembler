import digest_util as dig
import generator_util as gen


def generate_valid_maps_from_seq(sequences, enzyme, quant, coverage=25, seed=4231):
    # digest with enzyme
    genome_seq = []
    for idx, gen_iter in enumerate(sequences, start=1):
        genome_seq.append(['chr ' + str(idx) + gen_iter[0], dig.digest_with_enzyme(gen_iter[1], enzyme=enzyme)])

    # sample genome strands
    seq = []
    for gen_iter in genome_seq:
        seq.extend(gen.generate_errorfree_distribued_start_end_on_1(
                gen_iter, one_map_len=1000000, std_dev=300000, coverage=coverage, seed=seed))

    # quantize
    maps = []
    for dig_iter in seq:
        maps.append([dig_iter[0], dig.gen_bin_map_from_digest(dig_iter[1], quantize=int(quant))])

    return maps


def digest_and_quantize(sequences, enzyme, quant):
    digest = []
    for idx, seq_it in enumerate(sequences, start=1):
        digest.append(['chr ' + str(idx) + seq_it[0], dig.digest_with_enzyme(seq_it[1], enzyme=enzyme)])

    maps = []
    for dig_it in digest:
        maps.append([dig_it[0], dig.gen_bin_map_from_digest(dig_it[1], quantize=int(quant))])

    return maps


def generate_maps_distributed_from_seq(sequences, enzyme, quant, length, std_dev, coverage):
    maps = digest_and_quantize(sequences, enzyme, quant)

    result = []
    for i in maps:
        result.extend(
                gen.generate_errorfree_distribued(i, one_map_len=int(length), std_dev=int(std_dev), coverage=coverage))

    return result


def generate_maps_shifted_from_seq(sequences, enzyme, quant, length, shift):
    maps = digest_and_quantize(sequences, enzyme, quant)

    result = []
    for i in maps:
        result.extend(gen.generate_errorfree_overlaping(i, one_map_len=int(length), shift=int(shift)))

    return result
