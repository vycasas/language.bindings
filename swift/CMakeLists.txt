cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(swiftlib C)

if (NOT APPLE)
    message(FATAL_ERROR "Only Apple platforms are supported for Swift projects.")
    return ()
endif ()

if (NOT TARGET clib)
    message(FATAL_ERROR "Project objclib requires building clib first.")
    return ()
endif ()

set(LB_SWIFT_COMPILER "/usr/bin/swiftc")

find_library(FOUNDATION Foundation)

if (FOUNDATION-NOTFOUND)
    message(FATAL_ERROR "Foundation library not found.")
    return ()
endif ()

message(STATUS "Building project: swiftlib")

set(LB_SWIFTLIB_OUTPUT_DIR $<TARGET_FILE_DIR:clib>)
set(LB_SWIFTLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_SWIFTLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_SWIFTLIB_BRIDGING_HEADER_FILE "${LB_SWIFTLIB_HEADER_DIR}/dotslashzero/swiftlib/bridgingheader.h")
set(LB_SWIFTLIB_SOURCE_FILES "${LB_SWIFTLIB_SOURCE_DIR}/dotslashzero/swiftlib/SwiftLib.swift")

set(LB_SWIFTLIB_COMPILE_OPTIONS
    -warnings-as-errors
    -warn-implicit-overrides
    -Xcc -fpic -Xcc -fPIC)
set(LB_SWIFTLIB_COMPILE_OPTIONS_DEBUG
    -Onone -g
    -D DEBUG)
set(LB_SWIFTLIB_COMPILE_OPTIONS_RELEASE
    -O -gnone
    -D NDEBUG)
set(LB_SWIFTLIB_COMPILE_OPTIONS_MINSIZEREL
    -Osize -gnone
    -D NDEBUG)
set(LB_SWIFTLIB_COMPILE_OPTIONS_RELWITHDEBINFO
    -O -g
    -D NDEBUG)

set(LB_SWIFTLIB_LINK_OPTIONS
    -L ${LB_SWIFTLIB_OUTPUT_DIR} -lclib
    -framework Foundation
    -lobjc -lc)

get_target_property(LB_CLIB_INCLUDE_DIRECTORIES clib INCLUDE_DIRECTORIES)
foreach (LB_CLIB_INCLUDE_DIRECTORY ${LB_CLIB_INCLUDE_DIRECTORIES})
    set(LB_SWIFTLIB_COMPILE_OPTIONS
        ${LB_SWIFTLIB_COMPILE_OPTIONS}
        -I ${LB_CLIB_INCLUDE_DIRECTORY})
endforeach ()

set(LB_SWIFTLIB_VERSION "${LB_VERSION_MAJOR}.${LB_VERSION_MINOR}.${LB_VERSION_PATCH}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Info.plist")

set(LB_SWIFTLIB_FRAMEWORK_NAME "SwiftLib.framework")
set(LB_SWIFTLIB_FRAMEWORK_FILE "${LB_SWIFTLIB_OUTPUT_DIR}/${LB_SWIFTLIB_FRAMEWORK_NAME}")
set(LB_SWIFTLIB_FRAMEWORK_VERSION "A")

add_custom_target(
    swiftlib ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${LB_SWIFTLIB_FRAMEWORK_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Headers"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Modules/SwiftLib.swiftmodule"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources"
    COMMAND ${LB_SWIFT_COMPILER}
        -emit-library
        -o "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/SwiftLib"
        -emit-module
        -emit-module-path "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Modules/SwiftLib.swiftmodule/x86_64.swiftmodule"
        -emit-objc-header
        -emit-objc-header-path "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Headers/SwiftLib-Swift.h"
        ${LB_SWIFTLIB_COMPILE_OPTIONS}
        "$<$<CONFIG:Debug>:${LB_SWIFTLIB_COMPILE_OPTIONS_DEBUG}>"
        "$<$<CONFIG:Release>:${LB_SWIFTLIB_COMPILE_OPTIONS_RELEASE}>"
        "$<$<CONFIG:MinSizeRel>:${LB_SWIFTLIB_COMPILE_OPTIONS_MINSIZEREL}>"
        "$<$<CONFIG:RelWithDebInfo>:${LB_SWIFTLIB_COMPILE_OPTIONS_RELWITHDEBINFO}>"
        -import-objc-header ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
        ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_LINK_OPTIONS}
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources/Info.plist"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
    DEPENDS ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
    COMMAND_EXPAND_LISTS)

add_custom_command(
    TARGET swiftlib
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/SwiftLib" "${LB_SWIFTLIB_FRAMEWORK_FILE}/SwiftLib"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Headers" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Headers"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Modules" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Modules"
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Resources" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Resources"
    COMMAND ${CMAKE_COMMAND} -E rename "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/SwiftLib.dSYM" "${LB_SWIFTLIB_OUTPUT_DIR}/SwiftLib.dSYM" || true
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(swiftlib clib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
