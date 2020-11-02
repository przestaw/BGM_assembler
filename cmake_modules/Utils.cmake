find_package(Git QUIET)
# version
function(set_version)
    if (NOT DEFINED MAJOR_VERSION)
        set(MAJOR_VERSION 0)
    endif ()

    if (NOT DEFINED MINOR_VERSION)
        set(MINOR_VERSION 0)
    endif ()

    if (NOT DEFINED PATCH_VERSION)
        set(PATCH_VERSION 0)
    endif ()

    if (Git_FOUND)
        execute_process(
                COMMAND
                ${GIT_EXECUTABLE} rev-parse --short HEAD
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                RESULT_VARIABLE
                SHORT_HASH_RESULT
                OUTPUT_VARIABLE
                SHORT_HASH
                OUTPUT_STRIP_TRAILING_WHITESPACE)

        set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}_${SHORT_HASH} PARENT_SCOPE)
    else ()
        set(VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} PARENT_SCOPE)
    endif ()
endfunction()

# Immediately creates a directory
function(mkdir directory)
    if(NOT EXISTS "${directory}")
        execute_process(
                COMMAND ${CMAKE_COMMAND} -E make_directory ${directory}
                OUTPUT_QUIET
        )
    endif()
endfunction()

# Immediately creates a symbolic link between two files
function(symlink FROM TO)
    if(NOT EXISTS "${FROM}")
        return()
    endif()
    if(EXISTS "${TO}")
        return()
    endif()
    if(WIN32)
        set(linkme mklink)
        if(IS_DIRECTORY "${FROM}")
            set(argument "/d")
        else()
            set(argument "")
        endif()
    else()
        set(linkme "ln")
        set(argument "-s")
    endif()
    get_filename_component(WD "${TO}" PATH)
    get_filename_component(TO "${TO}" NAME)
    execute_process(COMMAND ${linkme} ${argument} ${FROM} ${TO}
            WORKING_DIRECTORY ${WD}
            )
endfunction()

# Adds to a variety of environment variables
function(add_to_envvar VARIABLE PATH)
    include(CMakeParseArguments)
    cmake_parse_arguments(envvar "PREPEND" "OS" "" ${ARGN})
    if(envvar_OS AND NOT ${${envvar_OS}})
        return()
    endif()
    if("$ENV{${VARIABLE}}" STREQUAL "")
        set(separator "")
    elseif(WIN32)
        set(separator ";")
    else()
        set(separator ":")
    endif()
    if(envvar_PREPEND)
        set(ENV{${VARIABLE}} "${PATH}${separator}$ENV{${VARIABLE}}")
    else()
        set(ENV{${VARIABLE}} "$ENV{${VARIABLE}}${separator}${PATH}")
    endif()
endfunction()