find_package(Python3 COMPONENTS Interpreter Development)
if(Python3_Interpreter_FOUND)
    #TODO find way to check for python libs
    add_custom_target(${PYTEST_UTIL}
            COMMAND
            Python3::Interpreter -m pytest ${PROJECT_SOURCE_DIR}/py_test/bgm_util
            WORKING_DIRECTORY
            ${PROJECT_SOURCE_DIR}/python)
endif()