import itertools
import os
from dataclasses import dataclass
import matplotlib.cm as cm
import matplotlib.pyplot as plt
import numpy as np
from bgm_util import PyBinaryMap

multi_colors = {
    "blue": 0.16,
    "green": 0.45,
    "red": 0.68,
    "orange": 0.65,
    "violet": 0.80,
    "empty": 1.0,
    "cmap": cm.get_cmap("gist_ncar")
}

simple_colors = {
    "blue": 0.92,
    "green": 0.75,
    "red": 0.1,
    "orange": 0.1,
    "violet": 1.0,
    "empty": 0.5,
    "cmap": cm.get_cmap("Spectral")
}


def arr_from_binmap(binmap, ref=False, colors=None):
    if colors is None:
        colors = multi_colors

    res = []
    for i in binmap:
        if i == 'n':
            res.append(colors['empty'])
        elif i == '0':
            if ref:
                res.append(colors['green'])
            else:
                res.append(colors['blue'])
        elif i == '1':
            if ref:
                res.append(colors['violet'])
            else:
                res.append(colors['red'])
    return res


def arr_diff(one, two, colors=None):
    if colors is None:
        colors = multi_colors

    res = []
    for (i, j) in itertools.zip_longest(one, two):
        if i == 'n' or j == 'n':
            res.append(colors['empty'])
        elif i == j:
            res.append(colors['green'])
        else:
            res.append(colors['red'])
    return res


class ComparedBitmaps:
    def __init__(self, one, two, colors=None):
        """
        Compares 2 maps by aligning them, and marking differences
        @param one: reference map
        @param two: map to be compared
        @param colors: compatible color dict
        """
        if type(one) != PyBinaryMap:
            raise TypeError(str(type(one)) + "!= BinaryMap")
        if type(two) != PyBinaryMap:
            raise TypeError(str(type(two)) + "!= BinaryMap")
        if colors is None:
            self.colors = multi_colors
        else:
            self.colors = colors

        # # # align
        self.diff, self.shift, self.overlap = PyBinaryMap.align_2_binmaps(one, two)

        # # # save quality
        # overlap part of smaller one
        self.overlap_part = self.overlap / min(len(one), len(two))
        self.part = self.overlap / max(len(one), len(two))

        # # # add padding at beginning if needed
        if self.shift > 0:
            self.two = self.shift * 'n' + two.string_map
            self.one = one.string_map
        elif self.shift < 0:
            self.two = two.string_map
            self.one = (-self.shift) * 'n' + one.string_map
        else:
            self.two = two.string_map
            self.one = one.string_map

        len_diff = len(self.one) - len(self.two)
        # # # add padding at the end if needed
        if len_diff > 0:
            self.two = self.two + 'n' * len_diff
        elif len_diff < 0:
            self.one = self.one + 'n' * (-len_diff)

    @property
    def quality(self):
        """
        No of diffrences between maps divided by overlap length
        @return: quality of overlap
        """
        return 1.0 - (self.diff / self.overlap)

    def export_file(self, filename="compare"):
        """
        Exports aligned binary maps as image
        @param filename: name of file being exported [suffix .png will be added]
        """
        data = []
        data.extend(np.tile(arr_from_binmap(self.one, colors=self.colors), 5))
        data.extend(np.tile(arr_from_binmap(self.two, colors=self.colors), 5))
        data.extend(np.tile(arr_diff(self.one, self.two, colors=self.colors), 5))

        height = 15

        os.makedirs(os.path.dirname(filename), exist_ok=True)
        plt.imsave(filename + '.png',
                   arr=np.array(data).reshape(height, int(len(data) / height)),
                   vmin=0., vmax=1., cmap=self.colors['cmap'])


class ComparedMultiBitmaps:
    def __init__(self, ref, others, truncate=True, colors=None):
        """
        Compares multiple map to reference by aligning them
        @param ref: reference map
        @param others: maps to be compared
        @param truncate: truncate to refmap len if True
        @param colors: compatible color dict
        """
        if type(ref) != PyBinaryMap:
            raise TypeError(str(type(ref)) + "!= BinaryMap")
        for j in others:
            if type(j) != PyBinaryMap:
                raise TypeError(str(type(j)) + "!= BinaryMap")
        if colors is None:
            self.colors = multi_colors
        else:
            self.colors = colors

        self.ref = ref.string_map
        self.ref_name = ref.name
        # # # align & save quality
        self.maps = []
        self.map_data = []

        @dataclass
        class MapData:
            name: str
            diff: int
            shift: int
            overlap: int
            overlap_part: float
            fo: int
            f: int
            r: int
            ro: int

        max_fo, max_ro = 0, 0

        for map_it in others:
            diff, shift, overlap = PyBinaryMap.align_2_binmaps(ref, map_it)
            # overlap part of smaller one
            overlap_part = overlap / min(len(ref), len(map_it))

            map_seq = map_it.string_map
            fo, f, r, ro = 0, 0, 0, 0
            # calc padding and possible overhangs
            if shift > 0:
                f = shift
                r = len(self.ref) - (len(map_seq) + shift)
                if r < 0:
                    ro = -r
                    r = 0
                else:
                    ro = 0

            elif shift < 0:
                fo = -shift
                f = 0
                r = fo + len(self.ref) - len(map_seq)
                if r < 0:
                    ro = -r
                    r = 0
                else:
                    ro = 0
            else:
                fo = 0
                f = 0
                r = len(self.ref) - len(map_seq)
                if r < 0:
                    r = 0
                    ro = -r
                else:
                    ro = 0

            self.map_data.append(MapData(map_it.name, diff, shift, overlap, overlap_part, fo, f, r, ro))

            max_fo = max_fo if max_fo > fo else fo
            max_ro = max_ro if max_ro > ro else ro

        # perform adjustments
        for map_iter, data in zip(others, self.map_data):
            map_str = map_iter.string_map

            if truncate:
                if data.fo > 0:
                    map_str = map_str[data.fo:]
                elif data.f > 0:
                    map_str = data.f * 'n' + map_str

                if data.ro > 0:
                    map_str = map_str[:-data.ro]
                elif data.r > 0:
                    map_str = map_str + data.r * 'n'
            else:
                if data.fo > 0:
                    map_str = (max_fo - data.fo) * 'n' + map_str
                elif data.f > 0:
                    map_str = (max_fo + data.f) * 'n' + map_str
                else:
                    map_str = max_fo * 'n' + map_str

                if data.ro > 0:
                    map_str = map_str + (max_ro - data.ro) * 'n'
                elif data.r > 0:
                    map_str = map_str + (max_ro + data.r) * 'n'
                else:
                    map_str = map_str + max_ro * 'n'

            self.maps.append(map_str)

        if not truncate:
            self.ref = max_fo * 'n' + self.ref + max_ro * 'n'

    @property
    def quality(self):
        """
        No of differences between maps divided by overlap length for each map
        @return: quality for each map in array
        """
        return [1.0 - (d.diff / d.overlap) for d in self.map_data]

    @property
    def diff(self):
        """
        No of differences between maps
        @return: no of differences for each map in array
        """
        return [d.diff for d in self.map_data]

    @property
    def shift(self):
        """
        Shift between maps from map begins
        @return: shift for each map in array
        """
        return [d.shift for d in self.map_data]

    @property
    def overlap(self):
        """
        Overlap between maps
        @return: overlap for each map in array
        """
        return [d.shift for d in self.map_data]

    @property
    def overlap_part(self):
        """
        Overlap size compared to smaller map len
        @return: overlap part for each map in array
        """
        return [d.shift for d in self.map_data]

    def export_file(self, filename="compare"):
        """
        Exports aligned binary maps as image
        @param filename: name of file being exported [suffix .png will be added]
        """
        data = []
        data.extend(np.tile(arr_from_binmap(self.ref, ref=True, colors=self.colors), 5))
        for map_iter in self.maps:
            data.extend(np.tile(arr_from_binmap(map_iter), 5))

        height = 5 * (1 + len(self.maps))
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        plt.imsave(filename + '.png',
                   arr=np.array(data).reshape(height, int(len(data) / height)),
                   vmin=0., vmax=1., cmap=self.colors['cmap'])

    def export_file_filtered(self, filename="compare", filter_q=0.85, error_enabled=True):
        """
        Exports aligned binary maps as image
        @param filename: name of file being exported [suffix .png will be added]
        @param filter_q: minimum quality of map to be exported
        @param error_enabled: raise exception if no map is matched
        """
        data = []
        map_names = [self.ref_name]
        map_c = 1
        # translate to 1-q, to avoid multiple substractions
        filter_q = 1 - filter_q

        data.extend(np.tile(arr_from_binmap(self.ref, ref=True, colors=self.colors), 5))

        for map_iter, map_data in zip(self.maps, self.map_data):
            if (map_data.diff / map_data.overlap) < filter_q:
                data.extend(np.tile(arr_from_binmap(map_iter), 5))
                map_names.append(map_data.name)
                map_c = map_c + 1

        if map_c > 1:
            # export image
            height = 5 * map_c
            os.makedirs(os.path.dirname(filename), exist_ok=True)
            plt.imsave(filename + '.png',
                       arr=np.array(data).reshape(height, int(len(data) / height)),
                       vmin=0., vmax=1., cmap=self.colors['cmap'])

            # export description
            mapfile = open(filename + '.mapnames', 'w+')
            for name in map_names:
                mapfile.write(name + '\n')

            mapfile.close()
        elif error_enabled:
            raise ValueError("No matched map !")
