cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(javalib_test)

if (NOT TARGET javalib)
    message(FATAL_ERROR "Cannot build test project without building the actual library to test (javalib).")
    return ()
endif ()

find_package(Java)

if (NOT JAVA_FOUND)
    message(FATAL_ERROR "Java platform not found.")
    return ()
endif ()

set(LB_JAVALIB_TEST_OUTPUT_DIR $<TARGET_FILE_DIR:javalib>)
set(LB_JAVALIB_TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB LB_JAVALIB_TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.java")

set(LB_JAVALIB_JAR_FILE "$<TARGET_FILE_DIR:javalib>/JavaLib.jar")

if (UNIX)
    add_custom_target(
        javalib_test ALL
        COMMAND${Java_JAVAC_EXECUTABLE} -d ${LB_JAVALIB_TEST_OUTPUT_DIR} -verbose -deprecation -Werror
            -cp ${LB_JAVALIB_TEST_OUTPUT_DIR}:${LB_JAVALIB_JAR_FILE}
            ${LB_JAVALIB_TEST_SOURCE_FILES}
        SOURCES ${LB_JAVALIB_TEST_SOURCE_FILES}
        DEPENDS ${LB_JAVALIB_TEST_SOURCE_FILES})
elseif (WIN32)
    add_custom_target(
        javalib_test ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d ${LB_JAVALIB_TEST_OUTPUT_DIR} -verbose -deprecation -Werror
            -cp "${LB_JAVALIB_TEST_OUTPUT_DIR};${LB_JAVALIB_JAR_FILE}"
            ${LB_JAVALIB_TEST_SOURCE_FILES}
        DEPENDS ${LB_JAVALIB_TEST_SOURCE_FILES}
        SOURCES ${LB_JAVALIB_TEST_SOURCE_FILES})
endif ()

add_dependencies(javalib_test javalib_jar)
