cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(nodejslib C)

if (NOT TARGET clib)
    message(FATAL_ERROR "Project nodejslib requires building clib first.")
    return ()
endif ()

if (NOT LB_NODEJS_HOME)
    message(FATAL_ERROR "LB_NODEJS_HOME must be set to build nodejslib.")
    return ()
endif ()

set(LB_NODEJS_INCLUDE_DIR "${LB_NODEJS_HOME}/include/node")
set(LB_NODEJS_LIBRARY)

message(STATUS "Building project: nodejslib")

set(LB_NODEJSLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_NODEJSLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_NODEJSLIB_HEADER_FILES
    "${LB_NODEJSLIB_HEADER_DIR}/dotslashzero/nodejslib/nodejslib.h")
set(LB_NODEJSLIB_SOURCE_FILES
    "${LB_NODEJSLIB_SOURCE_DIR}/dotslashzero/nodejslib/nodejslib.c")
set(LB_NODEJSLIB_NODEJS_SOURCE_FILE
    "${LB_NODEJSLIB_SOURCE_DIR}/dotslashzero/nodejslib/nodejslib.js")

source_group(headers\\dotslashzero\\nodejslib FILES ${LB_NODEJSLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\nodejslib FILES ${LB_NODEJSLIB_SOURCE_FILES} ${LB_NODEJSLIB_NODEJS_SOURCE_FILE})

if (UNIX)
    set(LB_NODEJSLIB_COMPILE_DEFINITIONS)
    set(LB_NODEJSLIB_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fvisibility=hidden
        -fpic -fPIC
        -fexceptions -frtti)
    set(LB_NODEJSLIB_LINK_OPTIONS)
    if (APPLE)
        set(LB_NODEJSLIB_LINK_OPTIONS ${LB_NODEJSLIB_LINK_OPTIONS} -undefined dynamic_lookup)
    endif ()
elseif (MSVC)
    set(LB_NODEJSLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_NODEJSLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(LB_NODEJSLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    nodejslib SHARED
    ${LB_NODEJSLIB_HEADER_FILES}
    ${LB_NODEJSLIB_SOURCE_FILES}
    ${LB_NODEJSLIB_NODEJS_SOURCE_FILE})

target_compile_definitions(
    nodejslib
    PRIVATE ${LB_NODEJSLIB_COMPILE_DEFINITIONS})
target_compile_options(
    nodejslib
    PRIVATE ${LB_NODEJSLIB_COMPILE_OPTIONS})
target_include_directories(
    nodejslib
    PUBLIC ${LB_NODEJSLIB_HEADER_DIR}
    PRIVATE ${LB_NODEJS_INCLUDE_DIR})
target_link_options(
    nodejslib
    PRIVATE ${LB_NODEJSLIB_LINK_OPTIONS})
target_link_libraries(
    nodejslib
    PUBLIC ${LB_NODEJS_LIBRARY} clib)

set_target_properties(nodejslib PROPERTIES OUTPUT_NAME "nodejslib_native")
set_target_properties(nodejslib PROPERTIES PREFIX "")
set_target_properties(nodejslib PROPERTIES SUFFIX ".node")

add_custom_command(
    TARGET nodejslib
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${LB_NODEJSLIB_NODEJS_SOURCE_FILE} "$<TARGET_FILE_DIR:nodejslib>/nodejslib.js")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
