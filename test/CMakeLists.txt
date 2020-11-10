add_executable(${TEST}
        ${LIB_INC} ${LIB_SRC} ${LIB_TMP} ${TEST_SRC} ${PROJECT_SOURCE_DIR}/test/test_main.cpp)
target_link_libraries(${TEST} ${EXTERNAL_LIBS} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(BGM-asm-testsuite "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST}")

add_executable(${TEST_PY_INT}
        ${LIB_INC} ${LIB_TMP} ${LIB_SRC} ${PY_SRC} ${PY_INC} ${PY_TEST_SRC} ${PROJECT_SOURCE_DIR}/test/test_main.cpp)
target_link_libraries(${TEST_PY_INT} ${EXTERNAL_LIBS} ${PYTHON_EXTERNAL} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(BGM-py-interface-testsuite "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_PY_INT}")

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(${BGM_CHECK} COMMAND ctest -R BGM-asm-testsuite
            --force-new-ctest-process --output-on-failure
            --build-config "$<CONFIGURATION>"
            DEPENDS ${TEST})
    add_custom_target(${PY_CHECK} COMMAND ctest -R BGM-py-interface-testsuite
            --force-new-ctest-process --output-on-failure
            --build-config "$<CONFIGURATION>"
            DEPENDS ${TEST_PY_INT})
else ()
    add_custom_target(${BGM_CHECK} COMMAND ctest -R BGM-asm-testsuite
            --force-new-ctest-process --output-on-failure
            DEPENDS ${TEST})
    add_custom_target(${PY_CHECK} COMMAND ctest -R BGM-py-interface-testsuite
            --force-new-ctest-process --output-on-failure
            DEPENDS ${TEST_PY_INT})
endif ()
