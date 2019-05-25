cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(cxxlib CXX)

set(CXXLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CXXLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(CXXLIB_HEADER_FILES
    "${CXXLIB_HEADER_DIR}/api.hxx")
set(CXXLIB_SOURCE_FILES
    "${CXXLIB_SOURCE_DIR}/api.cxx")

if (UNIX)
    set(CXXLIB_COMPILE_DEFINITIONS)
    set(CXXLIB_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpic -fPIC -fvisibility=hidden)
    set(CXXLIB_LINK_OPTIONS)
elseif (MSVC)
    set(CXXLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(CXXLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(CXXLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    cxxlib SHARED
    ${CXXLIB_HEADER_FILES}
    ${CXXLIB_SOURCE_FILES})
target_compile_definitions(
    cxxlib
    PRIVATE ${CXXLIB_COMPILE_DEFINITIONS})
target_compile_options(
    cxxlib
    PRIVATE ${CXXLIB_COMPILE_OPTIONS})
target_include_directories(
    cxxlib
    PUBLIC ${CXXLIB_HEADER_DIR})
target_link_options(
    cxxlib
    PRIVATE ${CXXLIB_LINK_OPTIONS})
target_link_libraries(
    cxxlib
    PUBLIC clib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
