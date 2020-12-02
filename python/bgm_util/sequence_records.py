from bgm_util import DigestUtil
from bgm_util import PyBinaryMap


class BaseRecord(object):
    """
    Wrapper class for any named sequence
    """
    def __init__(self, name, seq):
        """
        Wrapper class for any named sequence
        @param name: name of sequence
        @param seq: sequence
        """
        self.name = name
        self.sequence = seq

    def __len__(self):
        return len(self.sequence)

    def __getitem__(self, key):
        if isinstance(key, slice):
            start, stop, step = key.indices(len(self))
            if step is None:
                return BaseRecord(self.name, self.sequence[start:stop])
            elif step == 1:
                return BaseRecord(self.name, self.sequence[start:stop])
            else:
                raise AttributeError("use of step other than 1 is prohibited with " + type(self).__name__)
        elif isinstance(key, int):
            return self.sequence(key)
        else:
            raise TypeError('Invalid argument type: {}'.format(type(key)))

    def __str__(self):
        return '> ' + self.name + '\n' + self.sequence


class BinaryRecord(BaseRecord):
    """
    Wrapper class for binary named sequence
    """
    @classmethod
    def from_BaseRecord(cls, record):
        """
        Creates Binary record from BaseRecord
        @param record: BaseRecord with sequence
        """
        return BinaryRecord(record.name, record.sequence)

    def remove_0_ends(self):
        """
        Removes 0 overhands from sequence
        @return: self
        """
        self.sequence = DigestUtil.remove_overhang(self.sequence, '0')
        return self

    def quantize(self, quant):
        """
        Quantizes sequence with given quant value
        @param quant: quant len
        @return: self
        """
        self.sequence = DigestUtil.gen_bin_map_from_digest(self.sequence, quant)
        return self

    def to_BinaryMap(self):
        """
        Converts BinaryRecord to BinaryMap object
        @return: BinaryMap
        """
        return PyBinaryMap.from_string(self.name, self.sequence)


class DNARecord(BaseRecord):
    """
    Wrapper class for DNA named sequence
    """
    @classmethod
    def from_BaseRecord(cls, record):
        """
        Creates DNA record from BaseRecord
        @param record: BaseRecord with sequence
        """
        return DNARecord(record.name, record.sequence)

    def digest_with_enzyme(self, enzyme):
        """
        Digests sequence with given enzyme
        @param enzyme: enzyme
        @return: BinaryRecord with digest
        """
        return BinaryRecord(self.name,
                            DigestUtil.digest_with_enzyme(self.sequence, enzyme))

    def digest_with_pattern(self, pattern):
        """
        Digests sequence with given pattern
        @param pattern: pattern
        @return: BinaryRecord with digest
        """
        return BinaryRecord(self.name,
                            DigestUtil.digest_with_pattern(self.sequence, pattern))
