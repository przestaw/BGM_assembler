find_package(Python3 COMPONENTS Interpreter Development)
if(Python3_Interpreter_FOUND)
    #TODO find way to check for python libs

    # Python tests
    add_custom_target(${PYTEST_UTIL}
            COMMAND
            Python3::Interpreter -m pytest ${PROJECT_SOURCE_DIR}/py_test/bgm_util
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/python)

    # Cpp Python Module + extensions tests
    add_custom_target(${PYTEST_BGM}
            COMMAND
            Python3::Interpreter -m pytest ${PROJECT_SOURCE_DIR}/py_test/bgm_module
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/python)

    # depends Cpp module
    add_dependencies(${PYTEST_BGM} ${PYTHON_INT})

    # need to copy before run
    add_custom_command(
            TARGET ${PYTEST_BGM} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/bgm_module.so ${PROJECT_SOURCE_DIR}/python)
endif()