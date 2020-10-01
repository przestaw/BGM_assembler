function(clangformat_setup)
    if(NOT CLANGFORMAT_EXECUTABLE)
        set(CLANGFORMAT_EXECUTABLE clang-format)
    endif()

    if(NOT EXISTS ${CLANGFORMAT_EXECUTABLE})
        find_program(clangformat_executable_tmp ${CLANGFORMAT_EXECUTABLE})
        if(clangformat_executable_tmp)
            set(CLANGFORMAT_EXECUTABLE ${clangformat_executable_tmp})
            unset(clangformat_executable_tmp)

            foreach(clangformat_source ${ARGV})
                get_filename_component(clangformat_source ${clangformat_source} ABSOLUTE)
                list(APPEND clangformat_sources ${clangformat_source})
            endforeach()

            add_custom_target(format-project
                    COMMAND
                    ${CLANGFORMAT_EXECUTABLE}
                    -style=file
                    -i
                    ${clangformat_sources}
                    WORKING_DIRECTORY
                    ${CMAKE_SOURCE_DIR}
                    COMMENT
                    "Reformating with ${CLANGFORMAT_EXECUTABLE} ..."
                    )

            if(TARGET clangformat)
                add_dependencies(clangformat ${PROJECT_NAME}_clangformat)
            else()
                add_custom_target(clangformat DEPENDS ${PROJECT_NAME}_clangformat)
            endif()
        else()
            message("ClangFormat: ${CLANGFORMAT_EXECUTABLE} not found!")
        endif()
    endif()
endfunction()

# util
function(add_clagformat_target target)
    get_target_property(target_sources ${target} SOURCES)
    clangformat_setup(${target_sources})
endfunction()
