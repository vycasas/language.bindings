cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(clib C)

set(LB_CLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_CLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_CLIB_HEADER_FILES
    "${LB_CLIB_HEADER_DIR}/dotslashzero/clib/clib.h")
set(LB_CLIB_SOURCE_FILES
    "${LB_CLIB_SOURCE_DIR}/dotslashzero/clib/clib.c")

source_group(headers\\dotslashzero\\clib FILES ${LB_CLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\clib FILES ${LB_CLIB_SOURCE_FILES})

if (UNIX)
    set(LB_CLIB_COMPILE_DEFINITIONS)
    set(LB_CLIB_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fpic -fPIC
        -fvisibility=hidden)
    set(LB_CLIB_LINK_OPTIONS)
elseif (MSVC)
    set(LB_CLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_CLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp- /TC)
    set(LB_CLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    clib SHARED
    ${LB_CLIB_HEADER_FILES}
    ${LB_CLIB_SOURCE_FILES})

target_compile_definitions(
    clib
    PRIVATE ${LB_CLIB_COMPILE_DEFINITIONS})
target_compile_options(
    clib
    PRIVATE ${LB_CLIB_COMPILE_OPTIONS})
target_include_directories(
    clib
    PUBLIC ${LB_COMMON_HEADER_DIR} ${LB_CLIB_HEADER_DIR})
target_link_options(
    clib
    PRIVATE ${LB_CLIB_LINK_OPTIONS})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
