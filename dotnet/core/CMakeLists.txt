cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(dotnetcorelib)

if (NOT LB_BUILD_C)
    message(FATAL_ERROR "Project dotnetcorelib requires building clib first.")
    return ()
endif ()

set(LB_DOTNETCORELIB_BINARY_DIR "${CMAKE_BINARY_DIR}/$(Configuration)") # TODO: this will be different for Linux
set(LB_DOTNETCORELIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code")

set(LB_DOTNETCORELIB_GUID "{49D6814D-8947-42A7-8AF1-BD955AC74EEF}")
set(LB_DOTNETCORELIB_ASSEMBLY_NAME "DotSlashZero.DotNetCoreLib")
set(LB_DOTNETCORELIB_ROOT_NAMESPACE "DotSlashZero.DotNetCoreLib")

set(LB_DOTNETCORELIB_PLATFORM "x64")
if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(LB_DOTNETCORELIB_PLATFORM "x64")
elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(LB_DOTNETCORELIB_PLATFORM "x86")
else ()
    message(FATAL_ERROR "Unsupported platform for dotnetcorelib: ${CMAKE_VS_PLATFORM_NAME}")
    return ()
endif ()

file(RELATIVE_PATH LB_DOTNETCORELIB_ASSEMBLYINFO_CS_FILE
    ${CMAKE_CURRENT_BINARY_DIR} "${LB_DOTNETCORELIB_SOURCE_DIR}/AssemblyInfo.cs")
string(REPLACE "AssemblyInfo.cs" "*.cs"
    LB_DOTNETCORELIB_SOURCE_FILES_GLOB ${LB_DOTNETCORELIB_ASSEMBLYINFO_CS_FILE})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/dotnetcorelib.csproj.in"
    "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib.csproj")

if (MSVC)
    include_external_msproject(
        dotnetcorelib "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib.csproj"
        TYPE "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}" # C# project type
        GUID ${LB_DOTNETCORELIB_GUID}
        PLATFORM ${LB_DOTNETCORELIB_PLATFORM}
        clib)
else ()
    if (NOT LB_DOTNET_BIN_PATH)
        message(FATAL_ERROR
            "LB_DOTNET_BIN_PATH not defined. Please set it with the path to the dotnet binary executable.")
        return ()
    endif ()

    file(GLOB LB_DOTNETCORELIB_SOURCE_FILES "${LB_DOTNETCORELIB_SOURCE_DIR}/*.cs")
    add_custom_target(dotnetcorelib ALL
        COMMAND ${LB_DOTNET_BIN_PATH} build -configuration $<CONFIG> "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib.csproj"
            SOURCES ${LB_DOTNETCORELIB_SOURCE_FILES}
            DEPENDS ${LB_DOTNETCORELIB_SOURCE_FILES}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            VERBATIM)
    add_dependencies(dotnetcorelib clib)
endif ()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
