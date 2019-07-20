cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(objclib_test C)

if (NOT APPLE)
    message(FATAL_ERROR "Only Apple platforms are supported for Objective-C projects.")
    return ()
endif ()

if (NOT TARGET objclib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (objclib).")
    return ()
endif ()

find_library(FOUNDATION Foundation)

if (FOUNDATION-NOTFOUND)
    message(FATAL_ERROR "Foundation library not found.")
    return ()
endif ()

set(LB_OBJCLIB_TEST_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LB_OBJCLIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LB_OBJCLIB_TEST_HEADER_FILES)
set(LB_OBJCLIB_TEST_SOURCE_FILES
    "${LB_OBJCLIB_TEST_SOURCE_DIR}/Test.m")

source_group(headers FILES ${LB_OBJCLIB_TEST_HEADER_FILES})
source_group(sources FILES ${LB_OBJCLIB_TEST_SOURCE_FILES})

set(LB_OBJCLIB_TEST_COMPILE_DEFINITIONS)
set(LB_OBJCLIB_TEST_COMPILE_OPTIONS
    -W -Wall -Wextra -Werror
    -pedantic -pedantic-errors
    -fobjc-arc -fobjc-arc-exceptions
    -fpie -fPIE)
if (NOT XCODE)
    set(LB_OBJCLIB_TEST_COMPILE_OPTIONS -ObjC ${LB_OBJCLIB_COMPILE_OPTIONS})
endif ()
set(LB_OBJCLIB_TEST_LINK_OPTIONS)

add_executable(
    objclib_test
    ${LB_OBJCLIB_TEST_HEADER_FILES}
    ${LB_OBJCLIB_TEST_SOURCE_FILES})

target_compile_definitions(
    objclib_test
    PRIVATE ${LB_OBJCLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    objclib_test
    PRIVATE ${LB_OBJCLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    objclib_test
    PRIVATE ${LB_OBJCLIB_TEST_HEADER_DIR})
target_link_libraries(
    objclib_test
    PRIVATE $<TARGET_BUNDLE_DIR:objclib> objclib ${FOUNDATION})
target_link_options(
    objclib_test
    PRIVATE ${LB_OBJCLIB_TEST_LINK_OPTIONS})
