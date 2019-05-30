cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(dotnetlib CXX)

if (NOT MSVC)
    message(FATAL_ERROR "Only MSVC compilers are supported for .NET projects.")
    return ()
endif ()

set(DOTNETLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOTNETLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOTNETLIB_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(DOTNETLIB_HEADER_FILES "${DOTNETLIB_HEADER_DIR}/api_dotnet.hxx")
set(DOTNETLIB_SOURCE_FILES "${DOTNETLIB_SOURCE_DIR}/api_dotnet.cxx")
set(DOTNETLIB_RESOURCE_FILES
    "${DOTNETLIB_RESOURCE_DIR}/resource.hxx"
    "${DOTNETLIB_RESOURCE_DIR}/assemblyinfo.cxx"
    "${DOTNETLIB_RESOURCE_DIR}/app.ico"
    "${DOTNETLIB_RESOURCE_DIR}/app.rc")

set(DOTNETLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
set(DOTNETLIB_COMPILE_OPTIONS /W4 /WX /EHa /GR /openmp-)
set(DOTNETLIB_LINK_OPTIONS /WX)

add_library(
    dotnetlib SHARED
    ${DOTNETLIB_HEADER_FILES}
    ${DOTNETLIB_SOURCE_FILES})

target_compile_definitions(
    dotnetlib
    PRIVATE ${DOTNETLIB_COMPILE_DEFINITIONS})
target_compile_options(
    dotnetlib
    PRIVATE ${DOTNETLIB_COMPILE_OPTIONS})
target_include_directories(
    dotnetlib
    PRIVATE ${DOTNETLIB_HEADER_DIR})
target_link_options(
    dotnetlib
    PRIVATE ${DOTNETLIB_LINK_OPTIONS})
target_link_libraries(dotnetlib
    PUBLIC cxxlib)

set_target_properties(dotnetlib
    PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(dotnetlib
    PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")
set_target_properties(dotnetlib
    PROPERTIES OUTPUT_NAME "DotNetLib")
set_target_properties(dotnetlib
    PROPERTIES VS_DOTNET_REFERECES "System")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
