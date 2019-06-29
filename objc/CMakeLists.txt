cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(objclib C CXX)

if (NOT APPLE)
    message(FATAL_ERROR "Only Darwin compilers are supported for Objective-C projects.")
    return ()
endif ()

if (NOT LB_BUILD_C)
    message(FATAL_ERROR "Project objclib requires building clib first.")
    return ()
endif ()

find_library(FOUNDATION Foundation)

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
    -ObjC
    -W -Wall -Wextra -Werror
    -pedantic -pedantic-errors
    -fobjc-arc -fobjc-arc-exceptions
    -fpic -fPIC)
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
    PUBLIC clib ${FOUNDATION} objc c++ c)

set(LB_LB_OBJCLIB_VERSION "${LB_VERSION_MAJOR}.${LB_VERSION_MINOR}.${LB_VERSION_PATCH}")

set_target_properties(
    objclib PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${LB_LB_OBJCLIB_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER "net.dotslashzero.ObjCLib"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${LB_LB_OBJCLIB_VERSION}
    OUTPUT_NAME "ObjCLib"
    VERSION ${LB_LB_OBJCLIB_VERSION}
    SOVERSION ${LB_LB_OBJCLIB_VERSION}
    PUBLIC_HEADER "${LB_OBJCLIB_HEADER_DIR}/dotslashzero/objclib/ObjCLib.h")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
