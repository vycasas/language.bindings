cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(cxxlib CXX)

if (NOT LB_BUILD_C)
    message(FATAL_ERROR "C++ wrapper requires building C library.")
    return ()
endif ()

set(LB_CXXLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_CXXLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_CXXLIB_HEADER_FILES
    "${LB_CXXLIB_HEADER_DIR}/dotslashzero/cxxlib/cxxlib.hxx")
set(LB_CXXLIB_SOURCE_FILES
    "${LB_CXXLIB_SOURCE_DIR}/dotslashzero/cxxlib/cxxlib.cxx")

source_group(headers\\dotslashzero\\cxxlib FILES ${LB_CXXLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\cxxlib FILES ${LB_CXXLIB_SOURCE_FILES})

if (UNIX)
    set(LB_CXXLIB_COMPILE_DEFINITIONS)
    set(LB_CXXLIB_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fvisibility=hidden
        -fpic -fPIC
        -fexceptions -frtti)
    set(LB_CXXLIB_LINK_OPTIONS)
elseif (MSVC)
    set(LB_CXXLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_CXXLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(LB_CXXLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    cxxlib SHARED
    ${LB_CXXLIB_HEADER_FILES}
    ${LB_CXXLIB_SOURCE_FILES})

target_compile_definitions(
    cxxlib
    PRIVATE ${LB_CXXLIB_COMPILE_DEFINITIONS})
target_compile_options(
    cxxlib
    PRIVATE ${LB_CXXLIB_COMPILE_OPTIONS})
target_include_directories(
    cxxlib
    PUBLIC ${LB_CXXLIB_HEADER_DIR})
target_link_options(
    cxxlib
    PRIVATE ${LB_CXXLIB_LINK_OPTIONS})
target_link_libraries(
    cxxlib
    PUBLIC clib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
