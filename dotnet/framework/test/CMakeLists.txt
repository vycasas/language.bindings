cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetframeworklib_test CSharp)

set(LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

set(LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_FILES
    "${LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_DIR}/Program.cs")

source_group(sources FILES ${LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_FILES})

# TODO: until CMake has better C# support (e.g. LangVersion), command line is used for now

set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x64")
if ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x64")
elseif ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set(LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM "x86")
endif ()

set(LB_DOTNETFRAMEWORKLIB_TEST_COMPILE_FLAGS_DEBUG "")

add_custom_target(
    dotnetframeworklib_test ALL
    COMMAND ${CMAKE_CSharp_COMPILER}
        -langversion:8
        -platform:${LB_DOTNETFRAMEWORKLIB_TEST_PLATFORM}
        $<$<OR:$<CONFIG:DEBUG>,$<CONFIG:RELWITHDEBINFO>>:-debug>
        $<$<CONFIG:DEBUG>:-debug:full> $<$<CONFIG:RELWITHDEBINFO>:-debug:pdbonly>
        $<$<NOT:$<CONFIG:DEBUG>>:-optimize>
        $<IF:$<CONFIG:DEBUG>,-define:DEBUG,-define:TRACE>
        -target:exe -out:$<TARGET_FILE_DIR:dotnetframeworklib>/DotSlashZero.DotNetFrameworkLib.Test.exe
        -warnaserror+ -warn:4
        -reference:$<TARGET_FILE:dotnetframeworklib>
        ${LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_FILES}
    DEPENDS ${LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_FILES}
    SOURCES ${LB_DOTNETFRAMEWORKLIB_TEST_SOURCE_FILES}
    VERBATIM)

add_dependencies(dotnetframeworklib_test dotnetframeworklib)
