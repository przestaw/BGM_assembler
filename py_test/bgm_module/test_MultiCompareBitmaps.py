from bgm_module import CppBinaryMap
from bgm_module import MultiCompareBitmaps
import numpy as np


class TestMultiAlignMaps:
    def test_align_2_binmaps_1(self):
        map1 = CppBinaryMap('one',
                            '00100111')
        map2 = CppBinaryMap('two',
                            '11100100')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, False)

        expect = [[4, 4, 4, 2, 2, 1, 2, 2, 1, 1, 1],
                  [1, 1, 1, 0, 0, 1, 0, 0, 4, 4, 4]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]

    def test_align_2_binmaps_diff_size_1(self):
        map1 = CppBinaryMap('one',
                            '00000000000001111111111111111111111111111')
        map2 = CppBinaryMap('two',
                            '001111111111111111111111111111000000000')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, False)

        expect = [[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4,
                   4, 4, 4, 4, 4, 4, 4, 4],
                  #
                  [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
                   0, 0, 0, 0, 0, 0, 0, 0]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]

    def test_align_2_binmaps_diff_size_2(self):
        map1 = CppBinaryMap('one',
                            '111111000000000011111111111111')
        map2 = CppBinaryMap('two',
                            '0011111111111111000000000')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, False)

        expect = [[1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
                   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4],
                  #
                  [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                   0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]


    def test_align_2_binmaps_1_truncate(self):
        map1 = CppBinaryMap('one',
                            '00100111')
        map2 = CppBinaryMap('two',
                            '11100100')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, True)

        expect = [[2, 2, 1, 2, 2, 1, 1, 1],
                  [0, 0, 1, 0, 0, 4, 4, 4]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]

    def test_align_2_binmaps_diff_size_1_truncate(self):
        map1 = CppBinaryMap('one',
                            '00000000000001111111111111111111111111111')
        map2 = CppBinaryMap('two',
                            '001111111111111111111111111111000000000')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, True)

        expect = [[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, ],
                  #
                  [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, ]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]

    def test_align_2_binmaps_diff_size_2_truncate(self):
        map1 = CppBinaryMap('one',
                            '111111000000000011111111111111')
        map2 = CppBinaryMap('two',
                            '0011111111111111000000000')

        compareMaps = MultiCompareBitmaps(map1)
        compareMaps.addMapToCompare(map2)

        result = compareMaps.compareInt(4, 1, 0, 2, True)

        expect = [[1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
                   2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1],
                  #
                  [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
                   0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1]]

        for i in range(0, 2):
            for j in range(0, len(expect[0])):
                assert result[i][j] == expect[i][j]