file(GLOB ERROR_INC "${PROJECT_SOURCE_DIR}/include/error/*.h")
file(GLOB OM_DATA_INC "${PROJECT_SOURCE_DIR}/include/data/*.h")
file(GLOB LIB_INC "${PROJECT_SOURCE_DIR}/include/*.h")

include_directories(error data)

set(LIB_INC ${LIB_INC} ${OM_DATA_INC} ${ERROR_INC} ${ALGORITHMS_INC} PARENT_SCOPE)
