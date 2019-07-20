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

find_library(FOUNDATION Foundation)

if (FOUNDATION-NOTFOUND)
    message(FATAL_ERROR "Foundation library not found.")
    return ()
endif ()

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

if (XCODE)
    add_custom_target(
        swiftlib ALL
        COMMAND rm -fr ${LB_SWIFTLIB_FRAMEWORK_FILE}
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Headers"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Modules/SwiftLib.swiftmodule"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources"
        COMMAND swiftc
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
        COMMAND cp -v "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources/Info.plist"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        SOURCES ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
        DEPENDS ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
        COMMAND_EXPAND_LISTS)
else ()
    string(TOLOWER "${CMAKE_BUILD_TYPE}" LB_CMAKE_BUILD_TYPE)

    if ("${LB_CMAKE_BUILD_TYPE}" STREQUAL "debug")
        set(LB_SWIFTLIB_COMPILE_OPTIONS ${LB_SWIFTLIB_COMPILE_OPTIONS} ${LB_SWIFTLIB_COMPILE_OPTIONS_DEBUG})
    elseif ("${LB_CMAKE_BUILD_TYPE}" STREQUAL "release")
        set(LB_SWIFTLIB_COMPILE_OPTIONS ${LB_SWIFTLIB_COMPILE_OPTIONS} ${LB_SWIFTLIB_COMPILE_OPTIONS_RELEASE})
    elseif ("${LB_CMAKE_BUILD_TYPE}" STREQUAL "minsizerel")
        set(LB_SWIFTLIB_COMPILE_OPTIONS ${LB_SWIFTLIB_COMPILE_OPTIONS} ${LB_SWIFTLIB_COMPILE_OPTIONS_MINSIZEREL})
    elseif ("${LB_CMAKE_BUILD_TYPE}" STREQUAL "relwithdebinfo")
        set(LB_SWIFTLIB_COMPILE_OPTIONS ${LB_SWIFTLIB_COMPILE_OPTIONS} ${LB_SWIFTLIB_COMPILE_OPTIONS_RELWITHDEBINFO})
    else ()
        message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
        return ()
    endif ()

    add_custom_target(
        swiftlib ALL
        COMMAND rm -fr ${LB_SWIFTLIB_FRAMEWORK_FILE}
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Headers"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Modules/SwiftLib.swiftmodule"
        COMMAND mkdir -pv "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources"
        COMMAND swiftc
            -emit-library
            -o "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/SwiftLib"
            -emit-module
            -emit-module-path "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Modules/SwiftLib.swiftmodule/x86_64.swiftmodule"
            -emit-objc-header
            -emit-objc-header-path "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Headers/SwiftLib-Swift.h"
            ${LB_SWIFTLIB_COMPILE_OPTIONS}
            -import-objc-header ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
            ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_LINK_OPTIONS}
        COMMAND cp -v "${CMAKE_CURRENT_BINARY_DIR}/Info.plist" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}/Resources/Info.plist"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        SOURCES ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
        DEPENDS ${LB_SWIFTLIB_SOURCE_FILES} ${LB_SWIFTLIB_BRIDGING_HEADER_FILE}
        COMMAND_EXPAND_LISTS)
endif ()

add_custom_command(
    TARGET swiftlib
    POST_BUILD
    COMMAND ln -sfvh "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/${LB_SWIFTLIB_FRAMEWORK_VERSION}" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current"
    COMMAND ln -sfvh "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/SwiftLib" "${LB_SWIFTLIB_FRAMEWORK_FILE}/SwiftLib"
    COMMAND ln -sfvh "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Headers" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Headers"
    COMMAND ln -sfvh "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Modules" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Modules"
    COMMAND ln -sfvh "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/Resources" "${LB_SWIFTLIB_FRAMEWORK_FILE}/Resources"
    COMMAND mv -v "${LB_SWIFTLIB_FRAMEWORK_FILE}/Versions/Current/SwiftLib.dSYM" "${LB_SWIFTLIB_OUTPUT_DIR}/SwiftLib.dSYM" || true
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
