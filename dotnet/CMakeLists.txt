cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dotnetlib)

set(LB_DOTNETCORE_TARGET "netcoreapp2.2")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/core")

if (LB_BUILD_DOTNETFRAMEWORK)
    set(LB_DOTNETFRAMEWORK_TARGET "V4.8") # net48
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/framework")
endif ()
