cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(pythonlib_test C)

if (NOT TARGET pythonlib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (pythonlib).")
    return ()
endif ()

find_package(Python3 COMPONENTS Interpreter Development)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found")
    return ()
endif ()

set(LB_PYTHONLIB_TEST_OUTPUT_DIR $<TARGET_FILE_DIR:pythonlib>)
set(LB_PYTHONLIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LB_PYTHONLIB_TEST_OUTPUT_FILE "${LB_PYTHONLIB_TEST_OUTPUT_DIR}/pythonlib_test.py")
set(LB_PYTHONLIB_TEST_SOURCE_FILE "${LB_PYTHONLIB_TEST_SOURCE_DIR}/test.py")

add_custom_target(
    pythonlib_test ALL
    COMMAND ${CMAKE_COMMAND} -E remove -f ${LB_PYTHONLIB_TEST_OUTPUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E copy ${LB_PYTHONLIB_TEST_SOURCE_FILE} ${LB_PYTHONLIB_TEST_OUTPUT_FILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${LB_PYTHONLIB_TEST_SOURCE_FILE}
    DEPENDS ${LB_PYTHONLIB_TEST_SOURCE_FILE})

add_dependencies(pythonlib_test pythonlib)
