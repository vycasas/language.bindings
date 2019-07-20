cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetcorelib_test)

if (NOT TARGET dotnetcorelib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (dotnetcorelib).")
    return ()
endif ()

get_property(LB_IS_MULTI_CONFIG_IDE GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (LB_IS_MULTI_CONFIG_IDE AND (NOT MSVC))
    message(FATAL_ERROR "Only MSVC generator is supported for multi config .NET Core project")
    return ()
endif ()

set(LB_DOTNETCORELIB_TEST_GUID "{4251CCD3-A4CB-49E6-A44D-CEB899B450AB}")
set(LB_DOTNETCORELIB_TEST_ASSEMBLY_NAME "DotSlashZero.DotNetCoreLib.Test")
set(LB_DOTNETCORELIB_TEST_ROOT_NAMESPACE "DotSlashZero.DotNetCoreLib.Test")

if (MSVC)
    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set(LB_DOTNETCORELIB_TEST_PLATFORM "x64")
    elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        set(LB_DOTNETCORELIB_TEST_PLATFORM "x86")
    else ()
        message(FATAL_ERROR "Unsupported platform for dotnetcorelib_test: ${CMAKE_VS_PLATFORM_NAME}")
        return ()
    endif ()
else ()
    set(LB_DOTNETCORELIB_TEST_PLATFORM ${LB_DOTNET_PLATFORM})
endif ()

set(LB_DOTNETCORELIB_TEST_OUTPUT_DIR ${LB_DOTNETCORELIB_OUTPUT_DIR})
set(LB_DOTNETCORELIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(RELATIVE_PATH LB_DOTNETCORELIB_TEST_ASSEMBLYINFO_CS_FILE
    ${CMAKE_CURRENT_BINARY_DIR} "${LB_DOTNETCORELIB_TEST_SOURCE_DIR}/AssemblyInfo.cs")
string(REPLACE "AssemblyInfo.cs" "*.cs"
    LB_DOTNETCORELIB_TEST_SOURCE_FILES_GLOB ${LB_DOTNETCORELIB_TEST_ASSEMBLYINFO_CS_FILE})

set(LB_DOTNETCORELIB_ASSEMBLY_FILE_PATH
    "${LB_DOTNETCORELIB_OUTPUT_DIR}/${LB_DOTNETCORELIB_ASSEMBLY_NAME}.dll")

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
            --output ${LB_DOTNETCORELIB_TEST_OUTPUT_DIR}
            --configuration $<CONFIG> "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib_test.csproj"
            SOURCES ${LB_DOTNETCORELIB_TEST_SOURCE_FILES}
            DEPENDS ${LB_DOTNETCORELIB_TEST_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    add_dependencies(dotnetcorelib_test dotnetcorelib)
endif ()
