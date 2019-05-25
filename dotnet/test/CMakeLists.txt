cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(dotnetlib_test CXX)

if (NOT MSVC)
    message(FATAL_ERROR "Only MSVC compilers are supported for .NET projects.")
    return ()
endif ()

set(DOTNETLIB_TEST_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_dotnet_cxxcli")
set(DOTNETLIB_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_dotnet_cxxcli")
set(DOTNETLIB_TEST_RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_dotnet_cxxcli")

set(DOTNETLIB_TEST_HEADER_FILES)
set(DOTNETLIB_TEST_SOURCE_FILES
    "${DOTNETLIB_TEST_SOURCE_DIR}/program.cxx")
set(DOTNETLIB_TEST_RESOURCE_FILES
    "${DOTNETLIB_TEST_RESOURCE_DIR}/resource.hxx"
    "${DOTNETLIB_TEST_RESOURCE_DIR}/assemblyinfo.cxx"
    "${DOTNETLIB_TEST_RESOURCE_DIR}/app.ico"
    "${DOTNETLIB_TEST_RESOURCE_DIR}/app.rc")

set(DOTNETLIB_TEST_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
set(DOTNETLIB_TEST_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
set(DOTNETLIB_TEST_LINK_OPTIONS /WX)

add_executable(
    dotnetlib_test
    ${DOTNETLIB_TEST_HEADER_FILES}
    ${DOTNETLIB_TEST_SOURCE_FILES})
target_compile_definitions(
    dotnetlib_test
    PRIVATE ${DOTNETLIB_TEST_COMPILE_DEFINITIONS})
target_compile_options(
    dotnetlib_test
    PRIVATE ${DOTNETLIB_TEST_COMPILE_OPTIONS})
target_include_directories(
    dotnetlib_test
    PRIVATE ${DOTNETLIB_TEST_HEADER_DIR})
target_link_libraries(
    dotnetlib_test
    PRIVATE dotnetlib)
target_link_options(
    dotnetlib_test
    PRIVATE ${DOTNETLIB_TEST_LINK_OPTIONS})

set_target_properties(dotnetlib_test
    PROPERTIES COMMON_LANGUAGE_RUNTIME "")
set_target_properties(dotnetlib_test
    PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")
set_target_properties(dotnetlib_test
    PROPERTIES VS_DOTNET_REFERECES "$<TARGET_FILE:dotnetlib>;System")
