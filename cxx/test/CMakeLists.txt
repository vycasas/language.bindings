cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(cxxlib_test CXX)

set(LB_CXXLIB_TEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LB_CXXLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LB_CXXLIB_TEST_HEADER_FILES)
set(LB_CXXLIB_TEST_SOURCE_FILES
    "${LB_CXXLIB_TEST_SOURCE_DIR}/test.cxx")

source_group(headers FILES ${LB_CXXLIB_TEST_HEADER_FILES})
source_group(sources FILES ${LB_CXXLIB_TEST_SOURCE_FILES})

if (UNIX)
    set(LB_CXXLIB_TEST_COMPILE_DEFINITIONS)
    set(LB_CXXLIB_TEST_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fpie -fPIE
        -fexceptions -frtti)
    set(LB_CXXLIB_TEST_LINK_OPTIONS)
elseif (MSVC)
    set(LB_CXXLIB_TEST_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_CXXLIB_TEST_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(LB_CXXLIB_TEST_LINK_OPTIONS /WX)
endif ()

add_executable(
    cxxlib_test
    ${LB_CXXLIB_TEST_HEADER_FILES}
    ${LB_CXXLIB_TEST_SOURCE_FILES})

target_compile_definitions(
    cxxlib_test
    PRIVATE ${LB_CXXLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    cxxlib_test
    PRIVATE ${LB_CXXLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    cxxlib_test
    PRIVATE ${LB_CXXLIB_TEST_HEADER_DIR})
target_link_libraries(
    cxxlib_test
    PRIVATE cxxlib)
target_link_options(
    cxxlib_test
    PRIVATE ${LB_CXXLIB_TEST_LINK_OPTIONS})
