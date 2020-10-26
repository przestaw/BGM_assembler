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