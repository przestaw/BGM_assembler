from BGM_asm import AssemblyParams
from BGM_asm import AssemblyTask

params = AssemblyParams(1, 200, 0.5, 0.1, 0, True, False)

print("params = AssemblyParams(1, 200, 0.5, 0.1, 0, True, False)\nparams")
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