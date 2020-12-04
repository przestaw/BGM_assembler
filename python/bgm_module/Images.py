import numpy as np
from matplotlib import pyplot as plt, cm, axes
from _bgm_module import CppBinaryMap
from _bgm_module import CompareBitmaps

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
    if colors is None:
        colors = multi_colors
    compareMaps = CompareBitmaps(ref, test)
    compared = compareMaps.compareFloat(colors["empty"],
                                        colors["blue"],
                                        colors["red"],
                                        colors["green"])
    data = []
    data.extend(np.tile(compared[0], h))
    data.extend(np.tile(compared[1], h))
    data.extend(np.tile(compared[2], h))
    totH = h * 3
    data = np.array(data).reshape(totH, int(len(data) / totH))

    plt.imshow(data, interpolation='nearest', vmin=0, vmax=1, cmap=colors["cmap"])

    plt.yticks([h * 0.5 - 0.5, h * 1.5 - 0.5, h * 2.5 - 0.5], [ref.name, test.name, "overlap"], rotation=30)

    if save:
        plt.savefig("cmp"+ref.name+"-"+test.name+".png")


if __name__ == '__main__':
    map2 = CppBinaryMap("map2", "101011110000110101000111")
    map4 = CppBinaryMap("map4", "111000011010100011101")
    plot2bitmaps(map2, map4)



