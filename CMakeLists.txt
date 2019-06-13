cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(language.bindings)

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

message("language.bindings version ${LB_VERSION_STRING}")

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

if (APPLE)
    set(CMAKE_MACOSX_RPATH ${CMAKE_BINARY_DIR})
endif ()

option(LB_BUILD_C "Build C Library." ON)
option(LB_BUILD_CXX "Build C++ wrapper." OFF)
option(LB_BUILD_JAVA "Build Java wrapper." OFF)
if (APPLE)
    option(LB_BUILD_OBJC "Build Objective-C wrapper." OFF)
endif ()
if (MSVC)
    option(LB_BUILD_DOTNET "Build .NET wrapper." OFF)
endif ()

if (NOT CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (LB_BUILD_C)
    add_subdirectory(${CMAKE_SOURCE_DIR}/c)
endif ()

if (LB_BUILD_CXX)
    if (NOT LB_BUILD_C)
        message(FATAL_ERROR "C++ wrapper requires building C library.")
        return ()
    endif ()
    add_subdirectory(${CMAKE_SOURCE_DIR}/cxx)
endif ()

if (LB_BUILD_JAVA)
    if (NOT LB_BUILD_CXX)
        message(FATAL_ERROR "Java wrapper requires building C library and C++ wrapper.")
        return ()
    endif ()
    add_subdirectory(${CMAKE_SOURCE_DIR}/java)
endif ()

if (LB_BUILD_OBJC)
    add_subdirectory(${CMAKE_SOURCE_DIR}/objc)
endif ()

if (LB_BUILD_DOTNET)
    if (NOT LB_BUILD_CXX)
        message(FATAL_ERROR ".NET wrapper requires building C library and C++ wrapper.")
        return ()
    endif ()
    add_subdirectory(${CMAKE_SOURCE_DIR}/dotnet)
endif ()
