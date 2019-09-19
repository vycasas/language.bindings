cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(nodejslib_test)

if (NOT TARGET nodejslib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (nodejslib).")
    return ()
endif ()

set(LB_NODEJSLIB_TEST_OUTPUT_DIR $<TARGET_FILE_DIR:nodejslib>)
set(LB_NODEJSLIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LB_NODEJSLIB_TEST_OUTPUT_FILE "${LB_NODEJSLIB_TEST_OUTPUT_DIR}/nodejslib_test.js")
set(LB_NODEJSLIB_TEST_SOURCE_FILE "${LB_NODEJSLIB_TEST_SOURCE_DIR}/test.js")

add_custom_target(
    nodejslib_test ALL
    COMMAND ${CMAKE_COMMAND} -E remove -f ${LB_NODEJSLIB_TEST_OUTPUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${LB_NODEJSLIB_TEST_SOURCE_FILE} ${LB_NODEJSLIB_TEST_OUTPUT_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${LB_NODEJSLIB_TEST_SOURCE_FILE}
    DEPENDS ${LB_NODEJSLIB_TEST_SOURCE_FILE})

add_dependencies(nodejslib_test nodejslib)
