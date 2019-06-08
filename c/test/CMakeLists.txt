cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(clib_test C)

set(LB_CLIBTEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LB_CLIBTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LB_CLIBTEST_HEADER_FILES)
set(LB_CLIBTEST_SOURCE_FILES
    "${LB_CLIBTEST_SOURCE_DIR}/test.c")

if (UNIX)
    set(LB_CLIBTEST_COMPILE_DEFINITIONS)
    set(LB_CLIBTEST_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpie -fPIE)
    set(LB_CLIBTEST_LINK_OPTIONS)
elseif (MSVC)
    set(LB_CLIBTEST_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_CLIBTEST_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp- /TC)
    set(LB_CLIBTEST_LINK_OPTIONS /WX)
endif ()

add_executable(
    clib_test
    ${LB_CLIBTEST_HEADER_FILES}
    ${LB_CLIBTEST_SOURCE_FILES})

target_compile_definitions(
    clib_test
    PRIVATE ${LB_CLIBTEST_COMPILE_DEFINITIONS})
target_compile_options(
    clib_test
    PRIVATE ${LB_CLIBTEST_COMPILE_OPTIONS})
target_include_directories(
    clib_test
    PRIVATE ${LB_CLIBTEST_HEADER_DIR})
target_link_libraries(
    clib_test
    PRIVATE clib)
target_link_options(
    clib_test
    PRIVATE ${LB_CLIBTEST_LINK_OPTIONS})
