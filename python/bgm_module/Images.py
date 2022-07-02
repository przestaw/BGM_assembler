import numpy as np
from matplotlib import pyplot as plt, cm, axes
from bgm_module._bgm_module import CppBinaryMap
from bgm_module._bgm_module import CompareBitmaps
from bgm_module._bgm_module import MultiCompareBitmaps

multi_colors = {
    "blue": 0.16,
    "green": 0.45,
    "red": 0.68,
    "empty": 1.0,
    "cmap": cm.get_cmap("gist_ncar")
}

simple_colors = {
    "blue": 0.92,
    "green": 0.75,
    "red": 0.1,
    "empty": 0.5,
    "cmap": cm.get_cmap("Spectral")
}


def plot2bitmaps(ref: CppBinaryMap, test: CppBinaryMap, h: int = 4, colors=None, save=False):
    """
    Aligns and plots two binary maps along with their overlap row using the C++ extension module.
    @param ref: reference CppBinaryMap
    @param test: tested CppBinaryMap
    @param h: vertical pixel height of each rendered bar
    @param colors: optional color mapping dictionary
    @param save: if True, saves rendered PNG files to disk
    @return: 2D numpy array representing the rendered comparison image
    """
    if colors is None:
        colors = multi_colors
    compareMaps = CompareBitmaps(ref, test)
    compared = compareMaps.compareFloat(colors["empty"],
                                        colors["red"],
                                        colors["blue"],
                                        colors["green"])
    data = []
    data.extend(np.tile(compared[0], h))
    data.extend(np.tile(compared[1], h))
    data.extend(np.tile(compared[2], h))
    totH = h * 3
    data = np.array(data).reshape(totH, int(len(data) / totH))

    plt.imshow(data, interpolation='nearest', vmin=0, vmax=1, cmap=colors["cmap"])
    plt.yticks([h * 0.5 - 0.5, h * 1.5 - 0.5, h * 2.5 - 0.5], [ref.name, test.name, "overlap"], rotation=30)
    plt.gcf().tight_layout()

    if save:
        plt.imsave("cmp" + ref.name + "-" + test.name + ".raw.png", data, vmin=0, vmax=1, cmap=colors["cmap"])
        plt.savefig("cmp" + ref.name + "-" + test.name + ".png")

    return data


def plotMultiBitmaps(ref: CppBinaryMap, test: [], h: int = 4, truncate: bool = True, colors=None, save: bool = False):
    """
    Aligns and plots multiple binary maps against a reference map using the C++ extension module.
    @param ref: reference CppBinaryMap
    @param test: iterable of tested CppBinaryMap objects
    @param h: vertical pixel height of each rendered bar
    @param truncate: whether to truncate overhangs to the reference map length
    @param colors: optional color mapping dictionary
    @param save: target directory path to save PNG files, or False to skip saving
    @return: 2D numpy array representing the rendered multi-map alignment image
    """
    if colors is None:
        colors = multi_colors
    compareMaps = MultiCompareBitmaps(ref)

    for iMap in test:
        compareMaps.addMapToCompare(iMap)

    compared = compareMaps.compareFloat(colors["empty"],
                                        colors["red"],
                                        colors["blue"],
                                        colors["green"],
                                        truncate)
    data = []
    for i in range(0, len(compared)):
        data.extend(np.tile(compared[i], h))

    totH = h * (len(test) + 1)
    data = np.array(data).reshape(totH, int(len(data) / totH))

    plt.imshow(data, interpolation='nearest', vmin=0, vmax=1, cmap=colors["cmap"])
    plt.yticks([h * (i + 0.5) - 0.5 for i in range(0, len(compareMaps.names))], compareMaps.names, rotation=30)
    plt.gcf().tight_layout()

    if save:
        plt.imsave(save + "/cmp" + ref.name + "-multi.raw.png", data, vmin=0, vmax=1, cmap=colors["cmap"])
        plt.savefig(save + "/cmp" + ref.name + "-multi.png")

    return data
