cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(javalib_test)

file(GLOB LB_JAVALIB_TEST_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.java")
if (UNIX)
    add_custom_target(
        javalib_test ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d $<TARGET_FILE_DIR:javalib> -verbose -deprecation -Werror
            -cp $<TARGET_FILE_DIR:javalib>:$<TARGET_FILE_DIR:javalib>/JavaLib.jar ${LB_JAVALIB_TEST_SOURCE_FILES}
        SOURCES ${LB_JAVALIB_TEST_SOURCE_FILES}
        DEPENDS ${LB_JAVALIB_TEST_SOURCE_FILES})
elseif (WIN32)
    add_custom_target(
        javalib_test ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d $<TARGET_FILE_DIR:javalib> -verbose -deprecation -Werror
            -cp "$<TARGET_FILE_DIR:javalib>;$<TARGET_FILE_DIR:javalib>/JavaLib.jar" ${LB_JAVALIB_TEST_SOURCE_FILES}
        DEPENDS ${LB_JAVALIB_TEST_SOURCE_FILES}
        SOURCES ${LB_JAVALIB_TEST_SOURCE_FILES})
endif ()
add_dependencies(javalib_test javalib_jar)
