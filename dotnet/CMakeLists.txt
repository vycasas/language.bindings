cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetlib)

if (LB_BUILD_DOTNETFRAMEWORK)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/framework")
endif ()
