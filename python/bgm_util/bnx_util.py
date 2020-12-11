from bgm_util import FileIO
from bgm_util.digest_util import DigestUtil
from bgm_util import BaseRecord


def create_binary_maps(filename, quant=1000):
    """
    Creates binary maps from BNX file reads
    @param filename: filename
    @param quant: quant size
    @return: binary maps
    """
    bnx_records = FileIO.parse_BNX_file(filename)

    results = []
    for i in range(0, len(bnx_records)):
        digest = DigestUtil.generate_binary_string(bnx_records[i])
        binary_string = DigestUtil.gen_bin_map_from_digest(digest, quant)

        results.append(BaseRecord("read "+str(i), binary_string))

    return results
