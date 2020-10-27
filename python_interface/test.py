from BGM_asm import AssemblyParams
from BGM_asm import AssemblyTask
from BGM_asm import CppBinaryMap

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

map1 = CppBinaryMap("map1", "1010111100001101010001110101")
map2 = CppBinaryMap("map2", "10101111000011010100011")
map3 = CppBinaryMap("map3", "0101111000011010100011101")
map4 = CppBinaryMap("map4", "11100001101010001110101")

task = AssemblyTask(params)

task.addMap(map1)
task.addMap(map2)
task.addMap(map3)
task.addMap(map4)

task.execute()

res = task.getResult()

for i in res:
    print(i.toFastaRecord())
