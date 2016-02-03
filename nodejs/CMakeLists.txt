cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(nodejslib CXX)

# Ideally, it is recommended to use node-gyp (https://github.com/nodejs/node-gyp) to build Node.js addons, but because
# we are already using CMake, it is easier to just stick with it for this sample project.

if (UNIX)
    set(CXX_FLAGS "-std=c++11 -W -Wall -Wextra -Wno-unused-parameter -pedantic -pedantic-errors") # -Werror
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

# TODO: find a way to easily search for the headers and library
if (NODEJS_INCLUDE_DIR)
    message(STATUS "Using NODEJS_INCLUDE_DIR: ${NODEJS_INCLUDE_DIR}.")
else ()
    message(FATAL_ERROR "Please set NODEJS_INCLUDE_DIR CMake variable.")
endif ()

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${LINKER_FLAGS} ${SHARED_LIBRARY_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${NODEJS_INCLUDE_DIR}
)

add_library(nodejslib MODULE api_nodejs.cxx api_nodejs.hxx)
target_link_libraries(nodejslib cxxlib clib ${SYSTEM_LIBS})
set_target_properties(nodejslib PROPERTIES COMPILE_FLAGS "${SHARED_LIBRARY_CXX_FLAGS}")
set_target_properties(nodejslib PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")#"-Wl,--unresolved-symbols=ignore-all")
set_target_properties(nodejslib PROPERTIES OUTPUT_NAME "nodejslib_native")
set_target_properties(nodejslib PROPERTIES PREFIX "")
set_target_properties(nodejslib PROPERTIES SUFFIX ".node")
if (APPLE)
    set_target_properties(nodejslib PROPERTIES MACOSX_RPATH "${CMAKE_BINARY_DIR}")
endif ()
add_custom_command(
    TARGET nodejslib
    POST_BUILD
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/nodejslib.js ${CMAKE_BINARY_DIR}/nodejslib.js
    VERBATIM
)

add_custom_target(
    test_nodejs ALL
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/test/test.js ${CMAKE_BINARY_DIR}/test.js
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/test.js
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.js
    VERBATIM
)
add_dependencies(test_nodejs nodejslib)
