cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(javalib CXX)

if (NOT LB_BUILD_C)
    message(FATAL_ERROR "Project javalib requires building clib first.")
    return ()
endif ()

find_package(Java)
find_package(JNI)

if (NOT JAVA_FOUND)
    message(FATAL_ERROR "Java platform not found.")
    return ()
endif ()

if (NOT JNI_FOUND)
    message(FATAL_ERROR "JNI headers and libraries not found.")
    return ()
endif ()

message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")

set(LB_JAVALIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/lib/headers")
set(LB_JAVALIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/lib/sources")

set(LB_JAVALIB_HEADER_FILES
    "${LB_JAVALIB_HEADER_DIR}/dotslashzero/javalib/javalib.h")
set(LB_JAVALIB_SOURCE_FILES
    "${LB_JAVALIB_SOURCE_DIR}/dotslashzero/javalib/javalib.c")

source_group(headers\\dotslashzero\\javalib FILES ${LB_JAVALIB_HEADER_FILES})
source_group(sources\\dotslashzero\\javalib FILES ${LB_JAVALIB_SOURCE_FILES})

if (UNIX)
    set(LB_JAVALIB_COMPILE_DEFINITIONS)
    set(LB_JAVALIB_COMPILE_OPTIONS
        -W -Wall -Wextra -Werror
        -pedantic -pedantic-errors
        -fpic -fPIC
        -fvisibility=hidden)
    set(LB_JAVALIB_LINK_OPTIONS)
elseif (MSVC)
    set(LB_JAVALIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(LB_JAVALIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp- /TC)
    set(LB_JAVALIB_LINK_OPTIONS /WX)
endif ()

add_library(
    javalib SHARED
    ${LB_JAVALIB_HEADER_FILES}
    ${LB_JAVALIB_SOURCE_FILES})

target_compile_definitions(
    javalib
    PRIVATE ${LB_JAVALIB_COMPILE_DEFINITIONS})
target_compile_options(
    javalib
    PRIVATE ${LB_JAVALIB_COMPILE_OPTIONS})
target_include_directories(
    javalib
    PRIVATE ${LB_JAVALIB_HEADER_DIR} ${JNI_INCLUDE_DIRS})
target_link_options(
    javalib
    PRIVATE ${LB_JAVALIB_LINK_OPTIONS})
target_link_libraries(
    javalib
    PRIVATE clib ${JNI_LIBRARIES})

# Build the JAR wrappers for Java projects to consume
file(GLOB LB_JAVALIB_JAR_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/code/jar/net/dotslashzero/javalib/*.java")
add_custom_target(
    javalib_jar ALL
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR} -verbose -deprecation -Werror
        -cp ${CMAKE_CURRENT_SOURCE_DIR} ${LB_JAVALIB_JAR_SOURCE_FILES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/code/jar"
    SOURCES ${LB_JAVALIB_JAR_SOURCE_FILES}
    DEPENDS ${LB_JAVALIB_JAR_SOURCE_FILES}
    VERBATIM)

add_custom_command(
    TARGET javalib_jar
    POST_BUILD
    COMMAND ${Java_JAR_EXECUTABLE} cvf "$<TARGET_FILE_DIR:javalib>/JavaLib.jar" net/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM)

add_dependencies(javalib_jar javalib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
