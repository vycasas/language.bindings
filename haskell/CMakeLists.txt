cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(haskelllib C)

# To build a haskell shared object, we need to following compile and link commands:
# > ghc -outputdir $OUT_DIR -dynamic -fPIC -c MySource.hs
# > ghc -o libHaskellLib.so -dynamic -shared $OBJECT_FILES_TO_LINK

# To use the shared object created from above, we need to compile and link like below:
# > ghc -outputdir $OUT_DIR -dynamic -c Main.hs
# > ghc -o Main -dynamic -L$OUT_DIR -lHaskellLib

# TODO: move the find ghc to a CMake module...

# ABOUT LINKING
# READ: https://downloads.haskell.org/~ghc/latest/docs/html/users_guide/using-shared-libs.html#finding-shared-libs

if (WIN32)
    message(FATAL_ERROR "Building Haskell API is currently not supported on Windows platform.")
endif ()

set(
    GHC_PATHS
    /usr/bin
    /usr/local/bin
)

find_program(
    GHC_GHC_EXECUTABLE
    NAMES ghc
    PATHS ${GHC_PATHS}
)

if (GHC_GHC_EXECUTABLE)
    execute_process(
        COMMAND ${GHC_GHC_EXECUTABLE} --version
        RESULT_VARIABLE GHC_VERSION_RESULT
        OUTPUT_VARIABLE GHC_VERSION_OUT
        ERROR_VARIABLE GHC_VERSION_ERR
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "GHC found: ${GHC_VERSION_OUT}.")
else ()
    message(FATAL_ERROR "GHC not found.")
endif ()

if (UNIX)
    set(COPY_COMMAND cp)
    set(COPY_ARGS -rv)
    set(RM_COMMAND rm)
    set(RM_ARGS -fr)
    if (APPLE)
        set(SO_SUFFIX "dylib")
    else ()
        set(SO_SUFFIX "so")
    endif ()
elseif (WIN32)
    set(COPY_COMMAND copy)
    set(COPY_ARGS /y)
    set(RM_COMMAND del)
    set(RM_ARGS /q /s)
    set(SO_SUFFIX "dll")
endif ()

set(
    HASKELLLIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Library.hs
    ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Address.hs
    ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Person.hs
)

set(
    HASKELLLIB_OBJECTS
    ${CMAKE_CURRENT_BINARY_DIR}/HaskellModule/Library.o
    ${CMAKE_CURRENT_BINARY_DIR}/HaskellModule/Address.o
    ${CMAKE_CURRENT_BINARY_DIR}/HaskellModule/Person.o
)

add_custom_target(
    haskelllib ALL
    COMMAND ${RM_COMMAND} ${RM_ARGS}
        ${CMAKE_CURRENT_BINARY_DIR}/HaskellModule
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhaskelllib.${SO_SUFFIX}
    COMMAND echo "Building Haskell object haskell/HaskellModule/Library.o"
    COMMAND ${GHC_GHC_EXECUTABLE} -outputdir ${CMAKE_CURRENT_BINARY_DIR}
        -dynamic -fPIC -c ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Library.hs -I${CMAKE_SOURCE_DIR}
    COMMAND echo "Building Haskell object haskell/HaskellModule/Address.o"
    COMMAND ${GHC_GHC_EXECUTABLE} -outputdir ${CMAKE_CURRENT_BINARY_DIR}
        -dynamic -fPIC -c ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Address.hs -I${CMAKE_SOURCE_DIR}
    COMMAND echo "Building Haskell object haskell/HaskellModule/Person.o"
    COMMAND ${GHC_GHC_EXECUTABLE} -outputdir ${CMAKE_CURRENT_BINARY_DIR}
        -dynamic -fPIC -c ${CMAKE_CURRENT_SOURCE_DIR}/HaskellModule/Person.hs -I${CMAKE_SOURCE_DIR}
    COMMAND echo "Linking Haskell shared library libhaskelllib.${SO_SUFFIX}"
    COMMAND ${GHC_GHC_EXECUTABLE} -outputdir ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        -o ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhaskelllib.${SO_SUFFIX}
        -dynamic -shared ${HASKELLLIB_OBJECTS}
        -L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -lclib -dynload sysdep -optl -Wl,-rpath,. -optl -Wl,-rpath,'$ORIGIN'
    SOURCES ${HASKELLLIB_SOURCES}
    DEPENDS ${HASKELLLIB_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)

set(
    TEST_DEPENDENCIES
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libhaskelllib.${SO_SUFFIX}
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.hs
)
add_custom_target(
    test_haskell ALL
    COMMAND ${RM_COMMAND} ${RM_ARGS}
        ${CMAKE_CURRENT_BINARY_DIR}/test
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_haskell
    COMMAND echo "Building Haskell object haskell/test/Main.o"
    COMMAND ${GHC_GHC_EXECUTABLE} -outputdir ${CMAKE_CURRENT_BINARY_DIR}/test -dynamic -c
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test.hs
    COMMAND echo "Linking Haskell executable test_haskell"
    COMMAND ${GHC_GHC_EXECUTABLE} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_haskell -dynamic
        ${CMAKE_CURRENT_BINARY_DIR}/test/Main.o -L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -lhaskelllib -lclib -dynload sysdep
        -optl -Wl,-rpath,. -optl -Wl,-rpath,'$ORIGIN'
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.hs
    DEPENDS ${TEST_DEPENDENCIES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
)
