cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(pythonlib C)

if (NOT TARGET clib)
    message(FATAL_ERROR "Project pythonlib requires building clib first.")
    return ()
endif ()

find_package(Python3 COMPONENTS Interpreter Development)
if (NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found")
    return ()
endif ()

message(STATUS "Building project: pythonlib")

set(LB_PYTHONLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_PYTHONLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")

set(LB_PYTHONLIB_HEADER_FILES
    "${LB_PYTHONLIB_HEADER_DIR}/dotslashzero/pythonlib/pythonlib.h")
set(LB_PYTHONLIB_SOURCE_FILES
    "${LB_PYTHONLIB_SOURCE_DIR}/dotslashzero/pythonlib/pythonlib.c")
set(LB_PYTHONLIB_PYTHON_SOURCE_FILE
    "${LB_PYTHONLIB_SOURCE_DIR}/dotslashzero/pythonlib/pythonlib.py")

source_group(headers\\dotslashzero\\pythonlib FILES ${LB_PYTHONLIB_HEADER_FILES})
source_group(sources\\dotslashzero\\pythonlib FILES ${LB_PYTHONLIB_SOURCE_FILES} ${LB_PYTHONLIB_PYTHON_SOURCE_FILE})

if (UNIX)
    set(LB_PYTHONLIB_COMPILE_DEFINITIONS)
    set(LB_PYTHONLIB_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fvisibility=hidden
        -fpic -fPIC
        -fexceptions -frtti)
    set(LB_PYTHONLIB_LINK_OPTIONS)
elseif (MSVC)
    set(LB_PYTHONLIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_PYTHONLIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(LB_PYTHONLIB_LINK_OPTIONS /WX)
endif ()

add_library(
    pythonlib SHARED
    ${LB_PYTHONLIB_HEADER_FILES}
    ${LB_PYTHONLIB_SOURCE_FILES}
    ${LB_PYTHONLIB_PYTHON_SOURCE_FILE})

target_compile_definitions(
    pythonlib
    PRIVATE ${LB_PYTHONLIB_COMPILE_DEFINITIONS})
target_compile_options(
    pythonlib
    PRIVATE ${LB_PYTHONLIB_COMPILE_OPTIONS})
target_include_directories(
    pythonlib
    PUBLIC ${LB_PYTHONLIB_HEADER_DIR}
    PRIVATE ${Python3_INCLUDE_DIRS})
target_link_options(
    pythonlib
    PRIVATE ${LB_PYTHONLIB_LINK_OPTIONS})
target_link_libraries(
    pythonlib
    PUBLIC ${Python3_LIBRARIES} clib)

set_target_properties(pythonlib PROPERTIES OUTPUT_NAME "pythonlib_native")
set_target_properties(pythonlib PROPERTIES PREFIX "")
if (WIN32)
    set_target_properties(
        pythonlib
        PROPERTIES SUFFIX ".pyd")
else ()
    set_target_properties(
        pythonlib
        PROPERTIES SUFFIX ".so")
endif ()

add_custom_command(
    TARGET pythonlib
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy ${LB_PYTHONLIB_PYTHON_SOURCE_FILE} "$<TARGET_FILE_DIR:pythonlib>/pythonlib.py")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
