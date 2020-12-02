import bitarray as bit
import bitarray.util as bitutil
import copy


class PyBinaryMap:
    """
    Class for holding binary genome map
    """

    def __init__(self, name='empty', binary_map=bit.bitarray()):
        """
        Constructor
        @param: name: identifier or name of map
        @param: binary_map: bitarray with map
        """
        self.name = name
        self.bmap = binary_map

    def __len__(self):
        return len(self.bmap)

    @staticmethod
    def from_string(name, bitmap):
        """
        Constructs BinaryMap from string
        @param: name: identifier or name of map
        @param: bitmap: binary string with map
        @return: BinaryMap
        """
        return PyBinaryMap(name, bit.bitarray(bitmap))

    def copy(self):
        """
        Copies map
        @return: returns copy of a BinaryMap
        """
        # return BinaryMap(self.name, self.bmap.copy())
        return copy.deepcopy(self)

    def shift_left(self, shift):
        """
        Shifts left and discards rest of map
        @param: shift: no of bits to shift
        @return: self
        """
        del self.bmap[0:shift]
        return self

    def truncate(self, length):
        """
        Reduces len of map if length < len(map)
        @param: length: new length
        @return: self
        """
        diff = len(self) - length
        if diff > 0:
            self.bmap = self.bmap[:-diff]
        # elif diff < 0:
        # remake to resize ?
        return self

    def xor(self, other):
        """
        Performs xor of self and other and returns new BinaryMap
        @param: other: map to be xored NOTE: it's required to have equal length
        @return: xor result
        """
        new_name = 'xor ' + self.name + ' ^ ' + other.name
        return PyBinaryMap(new_name, self.bmap ^ other.bmap)

    def xor_count(self, other):
        """
        Performs xor of self and other and returns count of 1"
        @param: other: map to be xored NOTE: it's required to have equal length
        @return: count of differences between maps
        """
        # more efficient
        return bitutil.count_xor(self.bmap, other.bmap)

    def count(self, symbol=True):
        """
        Counts symbols in map
        @return: count of symbol
        """
        return self.bmap.count(symbol)

    @property
    def string_map(self):
        """
        String version of bmap
        @return:
        """
        return self.bmap.to01()

    @staticmethod
    def align_2_binmaps(ref, tested):
        """
        Aligns 2 maps for best alignment
        @param: ref: reference
        @param: tested: aligned map
        @return: pair (differences, shift)
        """

        flag = len(ref) > len(tested)
        diff_len = abs(len(ref) - len(tested))
        smaller = len(tested) if flag else len(ref)
        limit = int(smaller * 0.4)
        best, best_shift, align_sign, min_qual, overlap = smaller, 0, True, 1, 0

        def perform_xor(La, Ra, shift):
            # Shift
            R = Ra.copy().shift_left(shift).truncate(len(La))
            # Resize
            L = La.copy().truncate(len(R))
            # XOR
            result = L.xor_count(R)
            Q = result / len(L)

            return result, Q, len(L)

        def update(result, qual, shift, sign, ovlp):
            nonlocal best, best_shift, align_sign, min_qual, overlap
            if best > result:
                if qual < min_qual:
                    best = result
                    best_shift = shift
                    align_sign = sign
                    min_qual = qual
                    overlap = ovlp

        # Ref to Tested
        # [ R e f ]
        #      [ t e s t ]

        if flag:
            f_limit = limit + diff_len
        else:
            f_limit = limit

        for i in range(0, f_limit):
            res, qual, ovlp = perform_xor(tested, ref, i)
            update(res, qual, i, False, ovlp)

        # Tested to Ref
        #       [ R e f ]
        # [ t e s t ]
        if flag:
            f_limit = limit
        else:
            f_limit = limit + diff_len

        for i in range(0, f_limit):
            res, qual, ovlp = perform_xor(ref, tested, i)
            update(res, qual, i, True, ovlp)

        if align_sign:
            best_shift = -best_shift

        return best, best_shift, overlap
