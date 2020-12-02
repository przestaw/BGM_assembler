from bgm_util import BaseRecord
from bgm_util import DNARecord
from bgm_util import BinaryRecord
import pytest


class TestRecord:
    def test_slicing_error_1(self):
        test_record = BaseRecord("_", "123")

        with pytest.raises(AttributeError):
            val = test_record[1:10:3]

    def test_slicing_error_2(self):
        test_record = BaseRecord("_", "123")

        with pytest.raises(TypeError):
            val = test_record[(13, 1)]

        with pytest.raises(TypeError):
            val = test_record[2.14]

    def test_slicing_1(self):
        test_record = BaseRecord("name_2", "Any possible sequence of characters")

        res = test_record[4:]

        assert test_record.name == res.name
        assert "possible sequence of characters" == res.sequence

    def test_slicing_2(self):
        test_record = BaseRecord("name_2", "Any possible sequence of characters")

        res = test_record[:13]

        assert test_record.name == res.name
        assert "Any possible " == res.sequence

    def test_slicing_3(self):
        test_record = BaseRecord("name_2", "Any possible sequence of characters")

        res = test_record[13:21]

        assert test_record.name == res.name
        assert "sequence" == res.sequence


class TestBinaryRecord:

    def test_remove_0_ends(self):
        test_record = BinaryRecord('binary rec test', '00010111101101101000011100000000')

        test_record.remove_0_ends()

        assert "101111011011010000111" == test_record.sequence

    def test_quantize(self):
        test_record = BinaryRecord('binary rec test',
                                   '0001000000000001100000000000000000000010000000001011'
                                   '0000011000100000000001010000100000000000010100000000'
                                   '1100000000000000000000000010000000000000000000000000')

        test_record.quantize(10)

        assert "1101111111100100" == test_record.sequence

    def test_to_binary_map(self):
        test_record = BinaryRecord('binary rec test', '00010111101101101000011100000000')

        res = test_record.to_BinaryMap()

        assert test_record.sequence == res.string_map


class TestDNARecord:

    def test_digest_with_enzyme(self):
        test_record = DNARecord('dna rec test',
                                'AGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGTGGATTAAAAAAACACGAGTGATAGCAGCA'
                                'TTCTGAACTGGTTACCTGCCGTGAGTAAATTAAAATTTTATTGACTTAGGTCACTAAATACTTTAACCAA'
                                'TATAGGCATAGCGCACAGCACGAGAATTACAGAGTACACAACATCCATGAAACGCATTAGCACCACCAAA'
                                'ATTACCACCACCATCACCATTACCACAGGTAACGCACGAGTGACGCGTACAGGAAACACAGAAAAAAGAA'
                                'CCCGCACCTGACAGTGCGGGCTTTTTTTTTCGACCAAAGGTAACGAGGTAACAACCATGCGAGTGTTGAA'
                                'GTTCGGCGGTACATCAGTGGCAAATGCAGAACGTTTTCTGCGTGTTGCCCACGAGTGGAAAGCAATGCCA'
                                'AGGCAGGGGCAGGTGGCCACCGTCCTCTCTGCCCCCGCCAAAATCACCAACCACCTGGTGGCGATGATTG'
                                'AAAAAACCATTAGCGGCCAGGATGCTTTACCCAATATCAGCGATGCCGAACGTATTTTTGCCGAACTTTT'
                                'GACGGGACTCGCCGCCGCCCAGCCGGGGTTCCCGCTGGCGCAATTGAAAACTTTCGTCGATCAGGAATTT')

        res = test_record.digest_with_enzyme(enzyme='BssSI')

        expect = '00000000000000000000000000000000000000000000000000000100000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000010' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00001000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000001' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '0000000000000000000000000000000000000000000000000000000000000000000000'

        assert expect == res.sequence

    def test_digest_with_pattern(self):
        test_record = DNARecord('dna rec test',
                                'GACGGGACTCGCCGCCGCCCAGCCGGGGTTCCCGCTGGCGCAATTGAAAACTTTCGTCGATCAGGAATTT'
                                'GCCCAAATAAAACATGTCCTGCATGGCATTAGTTTGTTGGGGCAGTGCCCGGATAGCATCAACGCTGCGC'
                                'TGATTTGCCGTGGCGAGAAAATGTCGATCGCCATTATGGCCGGCGTATTAGAAGCGCGCGGTCACAACGT'
                                'TACTGTTATCGATCCGGTCGAAAAACTGCTGGCAGTGGGGCATTACCTCGAATCTACCGTCGATATTGCT'
                                'GAGTCCACCCGCCGTAGGGGTCAAAGCCGCATTCCGGCTGATCACATGGTGCTGATGGCAGGTTTCACCG'
                                'CCGGTAATGAAAAAGGCGAACTGGTGGTGCTTGGACGCAACGGTTCCGACTACTCTGCTGCGGTGCTGGC'
                                'TGCCTGTTTACGCGCCGATTGTTGCGAGATTTGGACGGACGTTGACGGGGTCTATACCTGCGACCCGCGT'
                                'CAGGTGCCCGATGCGAGGTTGTTGAAGTCGATGTCCTACCAGGAAGCGATGGAGCTTTCCTACTTCGGCG'
                                'CTAAAGTTCTTCACCCCCGCACCATTACCCCCATCGCCCAGTTCCAGATCCCTTGCCTGATTAAAAATAC'
                                'CGGAAATCCTCAAGCACCAGGTACGCTCATTGGTGCCAGCCGTGATGAAGACGAATTACCGGTCAAGGGC')

        res = test_record.digest_with_pattern(pattern='GGGGTC')

        expect = '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000100000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000010000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '00000000000000000000000000000000000000000000000000000000000000000000000000000000' \
                 '000000000000000000000000000000000000000000000000000000000000'

        assert expect == res.sequence
