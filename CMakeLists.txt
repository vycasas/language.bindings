cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(language.bindings)

enable_language(C)
enable_language(CXX)

file(READ "${CMAKE_SOURCE_DIR}/Version.txt" LB_VERSION_TXT_CONTENTS)
string(REPLACE "." ";" LB_VERSION_PARTS "${LB_VERSION_TXT_CONTENTS}")
list(LENGTH LB_VERSION_PARTS LB_VERSION_PARTS_LENGTH)
if (NOT ("${LB_VERSION_PARTS_LENGTH}" STREQUAL "3"))
    message(FATAL_ERROR "Invalid version format in Version.txt: \"${LB_VERSION_TXT_CONTENTS}\"")
    return ()
endif ()

list(GET LB_VERSION_PARTS 0 LB_VERSION_MAJOR)
list(GET LB_VERSION_PARTS 1 LB_VERSION_MINOR)
list(GET LB_VERSION_PARTS 2 LB_VERSION_PATCH)
set(LB_VERSION_EXTRA "")

string(REPLACE "-" ";" LB_VERSION_PATCH_PARTS "${LB_VERSION_PATCH}")
list(LENGTH LB_VERSION_PATCH_PARTS LB_VERSION_PATCH_PARTS_LENGTH)
if ("${LB_VERSION_PATCH_PARTS_LENGTH}" STRGREATER 1)
    list(GET LB_VERSION_PATCH_PARTS 0 LB_VERSION_PATCH)
    list(GET LB_VERSION_PATCH_PARTS 1 LB_VERSION_EXTRA)
endif ()

# this is done for validation purposes
set(LB_VERSION_STRING "${LB_VERSION_MAJOR}.${LB_VERSION_MINOR}.${LB_VERSION_PATCH}")
if (NOT ("${LB_VERSION_EXTRA}" STREQUAL ""))
    set(LB_VERSION_STRING "${LB_VERSION_STRING}-${LB_VERSION_EXTRA}")
endif ()

message(STATUS "Building language.bindings version ${LB_VERSION_STRING}")

set(LB_COMMON_HEADER_DIR "${CMAKE_BINARY_DIR}/headers")
configure_file(
    "${CMAKE_SOURCE_DIR}/version.h.in"
    "${LB_COMMON_HEADER_DIR}/dotslashzero/lb_common/version.h")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LB_CSHARP_LANG_VERSION "7.3")

if (APPLE)
    set(CMAKE_MACOSX_RPATH ${CMAKE_BINARY_DIR})
endif ()

option(LB_BUILD_C "Build C Library." ON)
option(LB_BUILD_CXX "Build C++ wrapper." OFF)
option(LB_BUILD_JAVA "Build Java wrapper." OFF)
if (APPLE)
    option(LB_BUILD_OBJC "Build Objective-C wrapper." OFF)
endif ()
option(LB_BUILD_DOTNET "Build .NET wrapper." OFF)
if (LB_BUILD_DOTNET)
    if (MSVC)
        option(LB_BUILD_DOTNETFRAMEWORK "Build .NET Framework wrapper." OFF)
    else ()
        set(LB_DOTNET_BIN_PATH "dotnet"
            CACHE FILEPATH "The file path to the dotnet binary executable of the .NET Core SDK")

        # TODO: auto check for i386 -> x86
        set(LB_DOTNET_PLATFORM "x64"
            CACHE STRING "The target platform for .NET Core projects (can be set to x64 or x86)")
    endif ()
endif ()

if (NOT CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (LB_BUILD_C)
    add_subdirectory("${CMAKE_SOURCE_DIR}/c")
endif ()

if (LB_BUILD_CXX)
    add_subdirectory("${CMAKE_SOURCE_DIR}/cxx")
endif ()

if (LB_BUILD_JAVA)
    add_subdirectory("${CMAKE_SOURCE_DIR}/java")
endif ()

if (LB_BUILD_OBJC)
    add_subdirectory("${CMAKE_SOURCE_DIR}/objc")
endif ()

if (LB_BUILD_DOTNET)
    add_subdirectory("${CMAKE_SOURCE_DIR}/dotnet")
endif ()
