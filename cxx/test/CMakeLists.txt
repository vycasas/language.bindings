cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(cxxlib_test CXX)

set(CXXLIB_TEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CXXLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CXXLIB_TEST_HEADER_FILES)
set(CXXLIB_TEST_SOURCE_FILES "${CXXLIB_TEST_SOURCE_DIR}/test.cxx")

if (UNIX)
    set(CXXLIB_TEST_COMPILE_DEFINITIONS)
    set(CXXLIB_TEST_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpie -fPIE)
    set(CXXLIB_TEST_LINK_OPTIONS)
elseif (MSVC)
    set(CXXLIB_TEST_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(CXXLIB_TEST_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(CXXLIB_TEST_LINK_OPTIONS /WX)
endif ()

add_executable(
    cxxlib_test
    ${CXXLIB_TEST_HEADER_FILES}
    ${CXXLIB_TEST_SOURCE_FILES})

target_compile_definitions(
    cxxlib_test
    PRIVATE ${CXXLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    cxxlib_test
    PRIVATE ${CXXLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    cxxlib_test
    PRIVATE ${CXXLIB_TEST_HEADER_DIR})
target_link_libraries(
    cxxlib_test
    PRIVATE cxxlib)
target_link_options(
    cxxlib_test
    PRIVATE ${CXXLIB_TEST_LINK_OPTIONS})
