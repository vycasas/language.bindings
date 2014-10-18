cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(javalib CXX)

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
    add_definitions(-DUNICODE -D_UNICODE)
endif ()

find_package(Java)
find_package(JNI)

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS} ${SHARED_LIBRARY_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${JAVA_INCLUDE_PATH}
    ${JAVA_INCLUDE_PATH2}
)

add_library(javalib SHARED jni/api_java.cxx jni/api_java.hxx)
target_link_libraries(javalib ${JAVA_JVM_LIBRARY} cxxlib clib ${SYSTEM_LIBS})
set_target_properties(javalib PROPERTIES COMPILE_FLAGS "${SHARED_LIBRARY_CXX_FLAGS}")

file(GLOB JAVA_SOURCES net/dotslashzero/javalib/*.java)
add_custom_target(
    javalibjar ALL
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_CURRENT_BINARY_DIR} -verbose -deprecation -Werror
        -cp ${CMAKE_BINARY_DIR} ${JAVA_SOURCES}
    SOURCES ${JAVA_SOURCES}
    DEPENDS ${JAVA_SOURCES}
    VERBATIM
)
add_custom_command(
    TARGET javalibjar
    POST_BUILD
    COMMAND ${Java_JAR_EXECUTABLE} cvf ${CMAKE_BINARY_DIR}/JavaLib.jar net/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)
add_dependencies(javalibjar javalib)

file(GLOB TEST_JAVA_SOURCES test/*.java)
if (UNIX)
    add_custom_target(
        test_java ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_BINARY_DIR} -verbose -deprecation -Werror
            -cp ${CMAKE_BINARY_DIR}:${CMAKE_BINARY_DIR}/JavaLib.jar ${TEST_JAVA_SOURCES}
        SOURCES ${TEST_JAVA_SOURCES}
        DEPENDS ${TEST_JAVA_SOURCES}
        VERBATIM
    )
else ()
    add_custom_target(
        test_java ALL
        COMMAND ${Java_JAVAC_EXECUTABLE} -d ${CMAKE_BINARY_DIR} -verbose -deprecation -Werror
            -cp "${CMAKE_BINARY_DIR};${CMAKE_BINARY_DIR}/JavaLib.jar" ${TEST_JAVA_SOURCES}
        DEPENDS ${TEST_JAVA_SOURCES}
        SOURCES ${TEST_JAVA_SOURCES}
        VERBATIM
    )
endif ()
add_dependencies(test_java javalibjar)
