cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetcorelib)

if (NOT TARGET clib)
    message(FATAL_ERROR "Project dotnetcorelib requires building clib first.")
    return ()
endif ()

get_property(LB_IS_MULTI_CONFIG_IDE GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (LB_IS_MULTI_CONFIG_IDE AND (NOT MSVC))
    message(FATAL_ERROR "Only MSVC generator is supported for multi config .NET Core project")
    return ()
endif ()

set(LB_DOTNETCORELIB_GUID "{49D6814D-8947-42A7-8AF1-BD955AC74EEF}")
set(LB_DOTNETCORELIB_ASSEMBLY_NAME "DotSlashZero.DotNetCoreLib")
set(LB_DOTNETCORELIB_ROOT_NAMESPACE "DotSlashZero.DotNetCoreLib")

if (MSVC)
    if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set(LB_DOTNETCORELIB_PLATFORM "x64")
    elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        set(LB_DOTNETCORELIB_PLATFORM "x86")
    else ()
        message(FATAL_ERROR "Unsupported platform for dotnetcorelib: ${CMAKE_VS_PLATFORM_NAME}")
        return ()
    endif ()
else ()
    set(LB_DOTNETCORELIB_PLATFORM ${LB_DOTNET_PLATFORM})
endif ()

get_target_property(LB_CLIB_OUTPUT_DIR clib LIBRARY_OUTPUT_DIRECTORY)
set(LB_DOTNETCORELIB_OUTPUT_DIR ${LB_CLIB_OUTPUT_DIR})
if (MSVC)
    # There must be a better way to do this
    set(LB_DOTNETCORELIB_OUTPUT_DIR "${LB_DOTNETCORELIB_OUTPUT_DIR}/$(Configuration)")
endif ()
set(LB_DOTNETCORELIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code")

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
        COMMAND echo "dotnet build begin"
        COMMAND ${LB_DOTNET_BIN_PATH} build
           --output ${LB_DOTNETCORELIB_OUTPUT_DIR}
           --configuration ${CMAKE_BUILD_TYPE} "${CMAKE_CURRENT_BINARY_DIR}/dotnetcorelib.csproj"
        COMMAND echo "dotnet build end"
        SOURCES ${LB_DOTNETCORELIB_SOURCE_FILES}
        DEPENDS ${LB_DOTNETCORELIB_SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_dependencies(dotnetcorelib clib)
endif ()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
