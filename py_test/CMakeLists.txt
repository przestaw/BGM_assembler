find_package(Python3 COMPONENTS Interpreter Development)
if(Python3_Interpreter_FOUND)
    #TODO find way to check for python libs

    set(PYTEST_COMMAND ${Python3_EXECUTABLE} -m pytest)

    # Python tests
    add_custom_target(${PYTEST_UTIL}
            COMMAND
            ${PYTEST_COMMAND} ${PROJECT_SOURCE_DIR}/py_test/bgm_util
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/python)

    # Cpp Python Module + extensions tests
    add_custom_target(${PYTEST_BGM}
            COMMAND
            ${CMAKE_COMMAND} -E copy
                ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/_bgm_module.so
            ../python/bgm_module/_bgm_module.so
            &&
            ${PYTEST_COMMAND} ${PROJECT_SOURCE_DIR}/py_test/bgm_module
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/python)

    # depends Cpp module
    add_dependencies(${PYTEST_BGM} ${PYTHON_INT})
endif()