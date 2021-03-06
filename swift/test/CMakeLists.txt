cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(swiftlib_test C)

if (NOT APPLE)
    message(FATAL_ERROR "Only Apple platforms are supported for Swift projects.")
    return ()
endif ()

if (NOT TARGET swiftlib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (objclib).")
    return ()
endif ()

set(LB_SWIFT_COMPILER "/usr/bin/swiftc")

find_library(FOUNDATION Foundation)

if (FOUNDATION-NOTFOUND)
    message(FATAL_ERROR "Foundation library not found.")
    return ()
endif ()

set(LB_SWIFTLIB_TEST_OUTPUT_DIR ${LB_SWIFTLIB_OUTPUT_DIR})
set(LB_SWIFTLIB_TEST_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LB_SWIFTLIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LB_SWIFTLIB_TEST_HEADER_FILES)
set(LB_SWIFTLIB_TEST_SOURCE_FILES
    "${LB_SWIFTLIB_TEST_SOURCE_DIR}/Test.swift")

set(LB_SWIFTLIB_TEST_COMPILE_OPTIONS
    -warnings-as-errors
    -warn-implicit-overrides
    -I "${LB_SWIFTLIB_FRAMEWORK_FILE}/Modules"
    -Xcc -fpie -Xcc -fPIE)
set(LB_SWIFTLIB_TEST_COMPILE_OPTIONS_DEBUG
    -Onone -g
    -D DEBUG)
set(LB_SWIFTLIB_TEST_COMPILE_OPTIONS_RELEASE
    -O -gnone
    -D NDEBUG)
set(LB_SWIFTLIB_TEST_COMPILE_OPTIONS_MINSIZEREL
    -Osize -gnone
    -D NDEBUG)
set(LB_SWIFTLIB_TEST_COMPILE_OPTIONS_RELWITHDEBINFO
    -O -g
    -D NDEBUG)

set(LB_SWIFTLIB_TEST_LINK_OPTIONS
    -F ${LB_SWIFTLIB_TEST_OUTPUT_DIR}
    -framework SwiftLib
    ${LB_SWIFTLIB_LINK_OPTIONS})

add_custom_target(
    swiftlib_test ALL
    COMMAND ${LB_SWIFT_COMPILER}
        -emit-executable
        -o "${LB_SWIFTLIB_TEST_OUTPUT_DIR}/swiftlib_test"
        ${LB_SWIFTLIB_TEST_COMPILE_OPTIONS}
        "$<$<CONFIG:Debug>:${LB_SWIFTLIB_TEST_COMPILE_OPTIONS_DEBUG}>"
        "$<$<CONFIG:Release>:${LB_SWIFTLIB_TEST_COMPILE_OPTIONS_RELEASE}>"
        "$<$<CONFIG:MinSizeRel>:${LB_SWIFTLIB_TEST_COMPILE_OPTIONS_MINSIZEREL}>"
        "$<$<CONFIG:RelWithDebInfo>:${LB_SWIFTLIB_TEST_COMPILE_OPTIONS_RELWITHDEBINFO}>"
        ${LB_SWIFTLIB_TEST_SOURCE_FILES}
        ${LB_SWIFTLIB_TEST_LINK_OPTIONS}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    SOURCES ${LB_SWIFTLIB_TEST_SOURCE_FILES}
    DEPENDS ${LB_SWIFTLIB_TEST_SOURCE_FILES}
    COMMAND_EXPAND_LISTS)

add_dependencies(swiftlib_test swiftlib)
