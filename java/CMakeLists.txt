cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(javalib CXX)

find_package(Java)
find_package(JNI)

set(JAVALIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jni")
set(JAVALIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/jni")

set(JAVALIB_HEADER_FILES
    "${JAVALIB_HEADER_DIR}/api_java.hxx")
set(JAVALIB_SOURCE_FILES
    "${JAVALIB_SOURCE_DIR}/api_java.cxx")

if (UNIX)
    set(JAVALIB_COMPILE_DEFINITIONS)
    set(JAVALIB_COMPILE_OPTIONS -W -Wall -Wextra -Werror -pedantic -pedantic-errors -fpic -fPIC -fvisibility=hidden)
    set(JAVALIB_LINK_OPTIONS)
elseif (MSVC)
    set(JAVALIB_COMPILE_DEFINITIONS -DUNICODE -D_UNICODE)
    set(JAVALIB_COMPILE_OPTIONS /W4 /WX /EHsc /GR /openmp-)
    set(JAVALIB_LINK_OPTIONS /WX)
endif ()

add_library(
    javalib SHARED
    ${JAVALIB_HEADER_FILES}
    ${JAVALIB_SOURCE_FILES})
target_compile_definitions(
    javalib
    PRIVATE ${JAVALIB_COMPILE_DEFINITIONS})
target_compile_options(
    javalib
    PRIVATE ${JAVALIB_COMPILE_OPTIONS})
target_include_directories(
    javalib
    PRIVATE ${JAVALIB_HEADER_DIR} ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})
target_link_options(
    javalib
    PRIVATE ${JAVALIB_LINK_OPTIONS})
target_link_libraries(
    javalib
    PRIVATE cxxlib)

file(GLOB JAVA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/net/dotslashzero/javalib/*.java")
add_custom_target(
    javalib_jar ALL
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR} -verbose -deprecation -Werror
        -cp ${CMAKE_CURRENT_SOURCE_DIR} ${JAVA_SOURCES}
    SOURCES ${JAVA_SOURCES}
    DEPENDS ${JAVA_SOURCES}
    VERBATIM
)
add_custom_command(
    TARGET javalib_jar
    POST_BUILD
    COMMAND ${Java_JAR_EXECUTABLE} cvf "$<TARGET_FILE_DIR:javalib>/JavaLib.jar" net/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)
add_dependencies(javalib_jar javalib)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
