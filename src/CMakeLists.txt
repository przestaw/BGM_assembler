file(GLOB OM_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB OM_DATA_SRC "${PROJECT_SOURCE_DIR}/src/data/*.cpp")
file(GLOB OM_ALG_SRC "${PROJECT_SOURCE_DIR}/src/algorithms/*.cpp")

set(LIB_SRC ${LIB_SRC} ${OM_SRC} ${OM_DATA_SRC} ${OM_ALG_SRC} PARENT_SCOPE)
