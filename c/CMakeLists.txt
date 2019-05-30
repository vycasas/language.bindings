cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(clib C)

#TODO: version generation

set(CLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CLIB_HEADER_FILES
    "${CLIB_HEADER_DIR}/api.h")
set(CLIB_SOURCE_FILES
    "${CLIB_SOURCE_DIR}/api.c")

if (UNIX)
    set(CLIB_COMPILE_DEFINITIONS)
    set(CLIB_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpic -fPIC -fvisibility=hidden)
    set(CLIB_LINK_OPTIONS)
elseif (MSVC)
    set(CLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(CLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp- /TC)
    set(CLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    clib SHARED
    ${CLIB_HEADER_FILES}
    ${CLIB_SOURCE_FILES})

target_compile_definitions(
    clib
    PRIVATE ${CLIB_COMPILE_DEFINITIONS})
target_compile_options(
    clib
    PRIVATE ${CLIB_COMPILE_OPTIONS})
target_include_directories(
    clib
    PUBLIC ${CLIB_HEADER_DIR})
target_link_options(
    clib
    PRIVATE ${CLIB_LINK_OPTIONS})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
