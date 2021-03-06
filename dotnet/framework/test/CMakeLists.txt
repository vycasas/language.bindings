cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetframeworklib_test)

if (NOT MSVC)
    message(FATAL_ERROR "Only MSVC compilers are supported for .NET Framework projects.")
    return ()
endif ()

if (NOT TARGET dotnetframeworklib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (dotnetframeworklib).")
    return ()
endif ()

set(LB_DOTNETFRAMEWORKLIB_TEST_GUID "{5CEB1F04-D7D6-40F6-BB94-B8A3AEE26603}")

set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x64")
if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x64")
elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x86")
else ()
    message(FATAL_ERROR "Unsupported platform for dotnetframeworklib_test: ${CMAKE_VS_PLATFORM_NAME}")
    return ()
endif ()

get_target_property(LB_DOTNETFRAMEWORKLIB_OUTPUT_DIR dotnetframeworklib LIBRARY_OUTPUT_DIRECTORY)
set(LB_DOTNETFRAMEWORKLIB_TEST_OUTPUT_DIR "${LB_DOTNETFRAMEWORKLIB_OUTPUT_DIR}/$(Configuration)")
set(LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# .csproj files does not support full absolute paths for source files
file(RELATIVE_PATH LB_DOTNETFRAMEWORKLIB_TEST_PROGRAM_CS_FILE
    ${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/Program.cs")
file(RELATIVE_PATH LB_DOTNETFRAMEWORKLIB_TEST_ASSEMBLYINFO_CS_FILE
    ${CMAKE_CURRENT_BINARY_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/AssemblyInfo.cs")

# MSVC old style .csproj requires paths to use back slashes
string(REPLACE "/" "\\"
    LB_DOTNETFRAMEWORKLIB_TEST_OUTPUT_DIR "${LB_DOTNETFRAMEWORKLIB_TEST_OUTPUT_DIR}/")
string(REPLACE "/" "\\"
    LB_DOTNETFRAMEWORKLIB_TEST_PROGRAM_CS_FILE ${LB_DOTNETFRAMEWORKLIB_TEST_PROGRAM_CS_FILE})
string(REPLACE "/" "\\"
    LB_DOTNETFRAMEWORKLIB_TEST_ASSEMBLYINFO_CS_FILE ${LB_DOTNETFRAMEWORKLIB_TEST_ASSEMBLYINFO_CS_FILE})

# would be nice to use the generator expressions, but the values are needed during configure time
get_property(LB_DOTNETFRAMEWORK_LIB_TARGET_FILE_NAME
    TARGET dotnetframeworklib
    PROPERTY OUTPUT_NAME)

set(LB_DOTNETFRAMEWORK_LIB_TARGET_FILE_PATH
    "${CMAKE_BINARY_DIR}/$(Configuration)/${LB_DOTNETFRAMEWORK_LIB_TARGET_FILE_NAME}.dll")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/dotnetframeworklib_test.csproj.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dotnetframeworklib_test.csproj")

include_external_msproject(
    dotnetframeworklib_test "${CMAKE_CURRENT_BINARY_DIR}/dotnetframeworklib_test.csproj"
    TYPE "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" # C# project type
    GUID ${LB_DOTNETFRAMEWORKLIB_TEST_GUID}
    PLATFORM ${LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM}
    dotnetframeworklib)
