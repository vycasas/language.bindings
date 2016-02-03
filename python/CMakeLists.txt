cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(pythonlib CXX)

if (UNIX)
    set(CXX_FLAGS "-std=c++11 -W -Wall -Wextra -Werror -pedantic -pedantic-errors")
    set(SHARED_LIBRARY_CXX_FLAGS "-fpic -fPIC -fvisibility=hidden")
    set(EXECUTABLE_CXX_FLAGS "-fpie -fPIE")
    set(LINKER_FLAGS "")
    set(SHARED_LIBRARY_LINKER_FLAGS "")
    set(EXECUTABLE_LINKER_FLAGS "")
    set(SYSTEM_LIBS c++ c)
elseif (MSVC)
    set(CXX_FLAGS "")
    set(SHARED_LIBRARY_CXX_FLAGS "")
    set(EXECUTABLE_CXX_FLAGS "")
    set(LINKER_FLAGS "")
    set(SHARED_LIBRARY_LINKER_FLAGS "")
    set(EXECUTABLE_LINKER_FLAGS "")
    set(SYSTEM_LIBS "")
endif ()

if (UNIX)
    set(COPY_COMMAND cp)
    set(COPY_ARGS -rv)
elseif (WIN32)
    set(COPY_COMMAND copy)
    set(COPY_ARGS /y)
endif ()

find_package(PythonInterp)
find_package(PythonLibs)

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${LINKER_FLAGS} ${SHARED_LIBRARY_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

add_library(pythonlib MODULE api_python.cxx api_python.hxx)
target_link_libraries(pythonlib ${PYTHON_LIBRARIES} cxxlib clib ${SYSTEM_LIBS})
set_target_properties(pythonlib PROPERTIES COMPILE_FLAGS "${SHARED_LIBRARY_CXX_FLAGS}")
set_target_properties(pythonlib PROPERTIES OUTPUT_NAME "pythonlib_native")
set_target_properties(pythonlib PROPERTIES PREFIX "")
if (APPLE)
    set_target_properties(pythonlib PROPERTIES MACOSX_RPATH "${CMAKE_BINARY_DIR}")
endif ()
add_custom_command(
    TARGET pythonlib
    POST_BUILD
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/pythonlib.py ${CMAKE_BINARY_DIR}/pythonlib.py
    VERBATIM
)

add_custom_target(
    test_python ALL
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/test/test.py ${CMAKE_BINARY_DIR}/test.py
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/test.py
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.py
    VERBATIM
)
add_dependencies(test_python pythonlib)
