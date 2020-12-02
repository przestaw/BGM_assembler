import matplotlib.pyplot as plt
import matplotlib.cm as cm
import numpy as np
from BGM_asm import AssemblyParams
from BGM_asm import AssemblyTask
from BGM_asm import CppBinaryMap
from BGM_asm import CompareBitmaps

params = AssemblyParams(1, 10, 0.5, 0.1, 1, True, False)

print("params = AssemblyParams(1, 200, 0.5, 0.1, 1, True, False)\nparams")
print("\t.threads = ", params.threads)
print("\t.minOverlap = ", params.minOverlap)
print("\t.minPart = ", params.minPart)
print("\t.threshold = ", params.threshold)
print("\t.repeats = ", params.repeats)
print("\t.threshold = ", params.threshold)
print("\t.markerPreference = ", params.markerPreference)
print("\t.reuseMaps = ", params.reuseMaps)

#import BGM_asm
#help(BGM_asm)

map1 = CppBinaryMap("map1", "10101111000011010100011101011")
map2 = CppBinaryMap("map2", "101011110000110101000111")
map3 = CppBinaryMap("map3",   "10111100001101010001")
map4 = CppBinaryMap("map4",      "111000011010100011101")
map5 = CppBinaryMap("map5",             "11010100011101011")

print("align")
compareMaps = CompareBitmaps(map2, map4)
print(compareMaps.compare(1, 2, 3, 4))
plt.imsave('name.png',
           arr=compareMaps,
           vmin=1., vmax=4., cmap=cm.get_cmap("gist_ncar"))


# print("align2")
# print(alignMapToRef(map3, map4, -1, 1, 0, 0))
# print("align3")
# print(alignMapToRef(map2, map4, -1, 1, 0, 0))

# task = AssemblyTask(params)
#
# task.addMap(map1)
# task.addMap(map2)
# task.addMap(map3)
# task.addMap(map4)
#
# task.execute()
#
# res = task.getResult()
#
# for i in res:
#     print(i.toFastaRecord())
