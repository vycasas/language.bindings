cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(objclib C)

if (NOT APPLE)
    message(FATAL_ERROR "Only Apple platforms are supported for Objective-C projects.")
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

message(STATUS "Building project: objclib")

set(LB_OBJCLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_OBJCLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_OBJCLIB_HEADER_FILES
    "${LB_OBJCLIB_HEADER_DIR}/dotslashzero/objclib/ObjCLib.h")
set(LB_OBJCLIB_SOURCE_FILES
    "${LB_OBJCLIB_SOURCE_DIR}/dotslashzero/objclib/ObjCLib.m")

source_group(headers\\dotslashzero\\objclib FILES ${LB_OBJCLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\objclib FILES ${LB_OBJCLIB_SOURCE_FILES})

set(LB_OBJCLIB_COMPILE_DEFINITIONS)
set(LB_OBJCLIB_COMPILE_OPTIONS
    -W -Wall -Wextra -Werror
    -pedantic -pedantic-errors
    -fobjc-arc -fobjc-arc-exceptions
    -fpic -fPIC)
if (NOT XCODE)
    set(LB_OBJCLIB_COMPILE_OPTIONS -ObjC ${LB_OBJCLIB_COMPILE_OPTIONS})
endif ()
set(LB_OBJCLIB_LINK_OPTIONS)

add_library(
    objclib SHARED
    ${LB_OBJCLIB_HEADER_FILES}
    ${LB_OBJCLIB_SOURCE_FILES})

target_compile_definitions(
    objclib
    PRIVATE ${LB_OBJCLIB_COMPILE_DEFINITIONS})
target_compile_options(
    objclib
    PRIVATE ${LB_OBJCLIB_COMPILE_OPTIONS})
target_include_directories(
    objclib
    PUBLIC ${LB_OBJCLIB_HEADER_DIR})
target_link_options(
    objclib
    PRIVATE ${LB_OBJCLIB_LINK_OPTIONS})
target_link_libraries(objclib
    PUBLIC clib ${FOUNDATION} objc c)

set(LB_OBJCLIB_VERSION "${LB_VERSION_MAJOR}.${LB_VERSION_MINOR}.${LB_VERSION_PATCH}")

set_target_properties(
    objclib PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION "A"
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${LB_OBJCLIB_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER "net.dotslashzero.ObjCLib"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${LB_OBJCLIB_VERSION}
    OUTPUT_NAME "ObjCLib"
    VERSION ${LB_OBJCLIB_VERSION}
    SOVERSION ${LB_OBJCLIB_VERSION}
    PUBLIC_HEADER "${LB_OBJCLIB_HEADER_DIR}/dotslashzero/objclib/ObjCLib.h")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
