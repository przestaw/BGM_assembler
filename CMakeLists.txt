cmake_minimum_required(VERSION 3.10)

# Set naming scheme 
set(PROJECT_NAME BGM-asm)
project(${PROJECT_NAME})

set(PROJECT_DESCRIPTION "Binary Maps Assembly prototype")

set(EXECUTABLE ${PROJECT_NAME}-program)
set(LIBRARY ${PROJECT_NAME})
set(TEST ${PROJECT_NAME}-test)
set(CHECK ${TEST}-check)
set(PYTHON_INT ${PROJECT_NAME}-python)

# Add custom cmake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules/finders")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

# Find external tools
find_package(LATEX)
find_package(Doxygen)

# Configure Clang-tidy
find_package(CLANG_TIDY)
# NOTE : checking is set for individual targets
set(CLANG_TIDY_CHECKS "cppcoreguidelines-*,performance-*,portability-*,-clang-analyzer-*,readability-*,cert-*,bugprone-*")

# Add Clang Format
include(UseClangFormat)

# Add CTest
include(CTest)
enable_testing()

# Add external libs : boost
find_package(Boost COMPONENTS program_options unit_test_framework python REQUIRED)
find_package(Threads REQUIRED)
set(EXTERNAL_LIBS "Boost::program_options" "Boost::unit_test_framework" "Threads::Threads")

# CXX standard
set(CMAKE_CXX_STANDARD 17)

# Configure Toolchain
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    if (USE_THREAD_CONTROL)
        set(CODE_CONTROL_FLAGS ${CODE_CONTROL_FLAGS} -Wthread-safety -fsanitize=thread)
    endif ()
    if (USE_CODE_CONTROL)
        set(CODE_CONTROL_FLAGS ${CODE_CONTROL_FLAGS} -fsanitize=memory -fsanitize=address -fsanitize=undefined)
    endif ()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g ${CODE_CONTROL_FLAGS} -Wextra-tokens -Wambiguous-member-template -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -finline-functions -fmerge-all-constants -fvectorize -flto")

    # find toolchain
    include(LLVMtoolchain)

    # configure toolchain
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld)

    set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_FINISH true)
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_FINISH true)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_BUILD_TYPE EQUAL Release)
        cmake_policy(SET CMP0069 NEW)
        include(CheckIPOSupported)
        check_ipo_supported(IPO_SUPPORT)
        if (IPO_SUPPORT)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        endif ()
    endif ()

    # using GCC
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -Og -g -Wshadow -Wall -pedantic -Wextra -Wold-style-cast")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -finline-functions -fdevirtualize -fmerge-constants")
    # find and set toolchain
    include(GNUtoolchain)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # TODO test
    message("MSVC support is not tested")

    # using MSVC (experimental)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /std:c++latest /W4 /Wall")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /W2")
endif ()

message(STATUS "C Compiler executable: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler executable: ${CMAKE_CXX_COMPILER}")
message(STATUS "Linker executable: ${CMAKE_LINKER}")

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# include dirs
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/external) # for mt4cpp

# find source files
file(GLOB_RECURSE EXT_SRC "${PROJECT_SOURCE_DIR}/external/*.hpp")
file(GLOB_RECURSE LIB_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIB_INC ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE LIB_TMP ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE TEST_SRC "${PROJECT_SOURCE_DIR}/test/*.cpp")

# Include src and test for library and test-check
add_subdirectory("test")

# library
add_library(${LIBRARY} ${LIB_SRC} ${LIB_INC} ${LIB_TMP})
add_dependencies(${LIBRARY} ${CHECK})

# Program executable
add_executable(${EXECUTABLE} main.cpp)
add_dependencies(${EXECUTABLE} ${LIBRARY})
target_link_libraries(${EXECUTABLE} ${LIBRARY})
target_link_libraries(${EXECUTABLE} ${EXTERNAL_LIBS})

# Python3 interface
find_package(PythonLibs 3.6 REQUIRED)

add_library(${PYTHON_INT} MODULE ${LIB_SRC} ${LIB_INC} ${LIB_TMP} python_interface/python_interface.cpp)

# Set up the libraries and header search paths for python3 interface
target_link_libraries(${PYTHON_INT} ${EXTERNAL_LIBS} "Boost::python" ${PYTHON_LIBRARIES})
target_include_directories(${PYTHON_INT} PRIVATE ${PYTHON_INCLUDE_DIRS})

# Clang Format
clangformat_setup(${LIB_SRC} ${LIB_INC} ${TEST_SRC} ${LIB_TMP} ${EXT_SRC} main.cpp python_interface/python_interface.cpp)
add_custom_target(build-all DEPENDS format-project ${PROGRAM} ${LIB} ${CHECK} ${TEST})

# Clang Tidy
if (CLANG_TIDY_FOUND)
    set_target_properties(${EXECUTABLE} ${LIBRARY}
            PROPERTIES
            CMAKE_CXX_CLANG_TIDY
            "${CLANG_TIDY_EXE}
            -checks=${CLANG_TIDY_CHECKS}")
    #    set(CMAKE_CXX_CLANG_TIDY
    #            ${CLANG_TIDY_EXE}
    #            -checks=${CLANG_TIDY_CHECKS} )
endif ()