cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(objclib_test C)

if (NOT APPLE)
    message(FATAL_ERROR "Only Darwin compilers are supported for Objective-C projects.")
    return ()
endif ()

find_library(FOUNDATION Foundation)

set(OBJCLIB_TEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OBJCLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OBJCLIB_TEST_HEADER_FILES)
set(OBJCLIB_TEST_SOURCE_FILES "${OBJCLIB_TEST_SOURCE_DIR}/test.m")

set(OBJCLIB_TEST_COMPILE_DEFINITIONS)
set(OBJCLIB_TEST_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fobjc-arc -fpie -fPIE)
set(OBJCLIB_TEST_LINK_OPTIONS)

add_executable(
    objclib_test
    ${OBJCLIB_TEST_HEADER_FILES}
    ${OBJCLIB_TEST_SOURCE_FILES})

target_compile_definitions(
    objclib_test
    PRIVATE ${OBJCLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    objclib_test
    PRIVATE ${OBJCLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    objclib_test
    PRIVATE ${OBJCLIB_TEST_HEADER_DIR})
target_link_libraries(
    objclib_test
    PRIVATE objclib)
target_link_options(
    objclib_test
    PRIVATE ${OBJCLIB_TEST_LINK_OPTIONS})
