cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(dotnetcorelib_test)

set(LB_DOTNETCORELIB_TEST_BINARY_DIR "${CMAKE_BINARY_DIR}/$(Configuration)")
set(LB_DOTNETCORELIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LB_DOTNETCORELIB_TEST_GUID "{4251CCD3-A4CB-49E6-A44D-CEB899B450AB}")
set(LB_DOTNETCORELIB_TEST_ASSEMBLY_NAME "DotSlashZero.DotNetCoreLib.Test")
set(LB_DOTNETCORELIB_TEST_ROOT_NAMESPACE "DotSlashZero.DotNetCoreLib.Test")

set(LB_DOTNETCORELIB_TEST_PLATFORM "x64")
if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(LB_DOTNETCORELIB_TEST_PLATFORM "x64")
elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(LB_DOTNETCORELIB_TEST_PLATFORM "x86")
else ()
    message(FATAL_ERROR "Unsupported platform for dotnetcorelib_test: ${CMAKE_VS_PLATFORM_NAME}")
    return ()
endif ()

file(RELATIVE_PATH LB_DOTNETCORELIB_TEST_ASSEMBLYINFO_CS_FILE
    ${CMAKE_CURRENT_BINARY_DIR} "${LB_DOTNETCORELIB_TEST_SOURCE_DIR}/AssemblyInfo.cs")
string(REPLACE "AssemblyInfo.cs" "*.cs"
    LB_DOTNETCORELIB_TEST_SOURCE_FILES_GLOB ${LB_DOTNETCORELIB_TEST_ASSEMBLYINFO_CS_FILE})

set(LB_DOTNETCORELIB_ASSEMBLY_FILE_PATH
    "${CMAKE_BINARY_DIR}/$(Configuration)/${LB_DOTNETCORELIB_ASSEMBLY_NAME}.dll")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/dotnetcorelib_test.csproj.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib_test.csproj")

if (MSVC)
    include_external_msproject(
        dotnetcorelib_test "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib_test.csproj"
        TYPE "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" # C# project type
        GUID ${LB_DOTNETCORELIB_TEST_GUID}
        PLATFORM ${LB_DOTNETCORELIB_TEST_PLATFORM}
        dotnetcorelib)
else ()
    if (NOT LB_DOTNET_BIN_PATH)
        message(FATAL_ERROR
            "LB_DOTNET_BIN_PATH not defined. Please set it with the path to the dotnet binary executable.")
        return ()
    endif ()

    file(GLOB LB_DOTNETCORELIB_TEST_SOURCE_FILES "${LB_DOTNETCORELIB_TEST_SOURCE_DIR}/*.cs")
    add_custom_target(dotnetcorelib_test ALL
        COMMAND ${LB_DOTNET_BIN_PATH} build
            -configuration $<CONFIG> "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib_test.csproj"
            SOURCES ${LB_DOTNETCORELIB_TEST_SOURCE_FILES}
            DEPENDS ${LB_DOTNETCORELIB_TEST_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)
    add_dependencies(dotnetcorelib_test dotnetcorelib)
endif ()
