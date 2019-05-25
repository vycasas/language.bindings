cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(javalib_test)

file(GLOB TEST_JAVA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.java")
if (UNIX)
    add_custom_target(
        javalib_test ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d $<TARGET_FILE_DIR:javalib> -verbose -deprecation -Werror
            -cp ${CMAKE_BINARY_DIR}:$<TARGET_FILE_DIR:javalib>/JavaLib.jar ${TEST_JAVA_SOURCES}
        SOURCES ${TEST_JAVA_SOURCES}
        DEPENDS ${TEST_JAVA_SOURCES}
        VERBATIM
    )
elseif (WIN32)
    add_custom_target(
        javalib_test ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d $<TARGET_FILE_DIR:javalib> -verbose -deprecation -Werror
            -cp "${CMAKE_BINARY_DIR};$<TARGET_FILE_DIR:javalib>/JavaLib.jar" ${TEST_JAVA_SOURCES}
        DEPENDS ${TEST_JAVA_SOURCES}
        SOURCES ${TEST_JAVA_SOURCES}
        VERBATIM
    )
endif ()
add_dependencies(javalib_test javalib_jar)
