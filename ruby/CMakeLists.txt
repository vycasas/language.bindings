cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(rubylib CXX)

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

find_package(Ruby)

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${LINKER_FLAGS} ${SHARED_LIBRARY_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${RUBY_INCLUDE_DIRS}
)

add_library(rubylib MODULE api_ruby.cxx api_ruby.hxx)
target_link_libraries(rubylib ${RUBY_LIBRARY} cxxlib clib ${SYSTEM_LIBS})
set_target_properties(rubylib PROPERTIES COMPILE_FLAGS "${SHARED_LIBRARY_CXX_FLAGS}")
set_target_properties(rubylib PROPERTIES OUTPUT_NAME "rubylibnative")
set_target_properties(rubylib PROPERTIES PREFIX "")
if (APPLE)
    # Important, Ruby modules for Mac OS requires .bundle suffix to be loaded.
    set_target_properties(rubylib PROPERTIES SUFFIX ".bundle")
endif ()
add_custom_command(
    TARGET rubylib
    POST_BUILD
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/rubylib.rb ${CMAKE_BINARY_DIR}/rubylib.rb
    VERBATIM
)

add_custom_target(
    test_ruby ALL
    COMMAND ${COPY_COMMAND} ${COPY_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/test/test.rb ${CMAKE_BINARY_DIR}/test.rb
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.rb
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/test.rb
    VERBATIM
)
add_dependencies(test_ruby rubylib)
