cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(clib_test C)

set(CLIB_TEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CLIB_TEST_HEADER_FILES)
set(CLIB_TEST_SOURCE_FILES
    "${CLIB_TEST_SOURCE_DIR}/test.c")

if (UNIX)
    set(CLIB_TEST_COMPILE_DEFINITIONS)
    set(CLIB_TEST_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpie fPIE)
    set(CLIB_TEST_LINK_OPTIONS)
elseif (MSVC)
    set(CLIB_TEST_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(CLIB_TEST_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp- /TC)
    set(CLIB_TEST_LINK_OPTIONS /WX)
endif ()

add_executable(
    clib_test
    ${CLIB_TEST_HEADER_FILES}
    ${CLIB_TEST_SOURCE_FILES})
target_compile_definitions(
    clib_test
    PRIVATE ${CLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    clib_test
    PRIVATE ${CLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    clib_test
    PRIVATE ${CLIB_TEST_HEADER_DIR})
target_link_libraries(
    clib_test
    PRIVATE clib)
target_link_options(
    clib_test
    PRIVATE ${CLIB_TEST_LINK_OPTIONS})
