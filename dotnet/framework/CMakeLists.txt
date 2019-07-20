cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetframeworklib CXX)

if (NOT MSVC)
    message(FATAL_ERROR "Only MSVC compilers are supported for .NET Framework projects.")
    return ()
endif ()

if (NOT TARGET cxxlib)
    message(FATAL_ERROR "Project dotnetframeworklib requires building cxxlib first.")
    return ()
endif ()

set(LB_DOTNETFRAMEWORKLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_DOTNETFRAMEWORKLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")
set(LB_DOTNETFRAMEWORKLIB_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")

set(LB_DOTNETFRAMEWORKLIB_HEADER_FILES
    "${LB_DOTNETFRAMEWORKLIB_HEADER_DIR}/dotslashzero/dotnetframeworklib/dotnetframeworklib.hxx")
set(LB_DOTNETFRAMEWORKLIB_SOURCE_FILES
    "${LB_DOTNETFRAMEWORKLIB_SOURCE_DIR}/dotslashzero/dotnetframeworklib/assemblyinfo.cxx"
    "${LB_DOTNETFRAMEWORKLIB_SOURCE_DIR}/dotslashzero/dotnetframeworklib/dotnetframeworklib.cxx")
set(LB_DOTNETFRAMEWORKLIB_RESOURCE_FILES
    "${LB_DOTNETFRAMEWORKLIB_RESOURCE_DIR}/resource.hxx"
    "${LB_DOTNETFRAMEWORKLIB_RESOURCE_DIR}/app.ico"
    "${LB_DOTNETFRAMEWORKLIB_RESOURCE_DIR}/app.rc")

source_group(headers\\dotslashzero\\dotnetframeworklib FILES ${LB_DOTNETFRAMEWORKLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\dotnetframeworklib FILES ${LB_DOTNETFRAMEWORKLIB_SOURCE_FILES})
source_group(resources FILES ${LB_DOTNETFRAMEWORKLIB_RESOURCE_FILES})

set(LB_DOTNETFRAMEWORKLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
set(LB_DOTNETFRAMEWORKLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
set(LB_DOTNETFRAMEWORKLIB_LINK_OPTIONS /WX)

add_library(
    dotnetframeworklib SHARED
    ${LB_DOTNETFRAMEWORKLIB_HEADER_FILES}
    ${LB_DOTNETFRAMEWORKLIB_SOURCE_FILES}
    ${LB_DOTNETFRAMEWORKLIB_RESOURCE_FILES})

target_compile_definitions(
    dotnetframeworklib
    PRIVATE ${LB_DOTNETFRAMEWORKLIB_COMPILE_DEFINITIONS})
target_compile_options(
    dotnetframeworklib
    PRIVATE ${LB_DOTNETFRAMEWORKLIB_COMPILE_OPTIONS})
target_include_directories(
    dotnetframeworklib
    PRIVATE ${LB_DOTNETFRAMEWORKLIB_HEADER_DIR})
target_link_options(
    dotnetframeworklib
    PRIVATE ${LB_DOTNETFRAMEWORKLIB_LINK_OPTIONS})
target_link_libraries(
    dotnetframeworklib
    PUBLIC cxxlib)

set_target_properties(dotnetframeworklib
    PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(dotnetframeworklib
    PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")
set_target_properties(dotnetframeworklib
    PROPERTIES OUTPUT_NAME "DotSlashZero.DotNetFrameworkLib")
set_target_properties(dotnetframeworklib
    PROPERTIES VS_DOTNET_REFERECES "System")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
