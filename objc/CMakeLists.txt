cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(objclib C CXX)

if (NOT APPLE)
    message(FATAL_ERROR "Only Darwin compilers are supported for Objective-C projects.")
    return ()
endif ()

if (NOT LB_BUILD_C)
    message(FATAL_ERROR "Project objclib requires building clib first.")
    return ()
endif ()

find_library(FOUNDATION Foundation)

set(LB_OBJCLIB_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/headers")
set(LB_OBJCLIB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/code/sources")
set(LB_OBJCLIB_HEADER_FILES "${LB_OBJCLIB_HEADER_DIR}/dotslashzero/objclib/ObjCLib.h")
set(LB_OBJCLIB_SOURCE_FILES "${LB_OBJCLIB_SOURCE_DIR}/dotslashzero/objclib/ObjCLib.m")

set(LB_OBJCLIB_COMPILE_DEFINITIONS)
set(LB_OBJCLIB_COMPILE_OPTIONS
    -ObjC
    -W -Wall -Wextra -Werror
    -pedantic -pedantic-errors
    -fobjc-arc -fobjc-arc-exceptions
    -fpic -fPIC)
set(LB_OBJCLIB_LINK_OPTIONS)

add_library(
    objclib SHARED
    ${LB_OBJCLIB_HEADER_FILES}
    ${LB_OBJCLIB_SOURCE_FILES})

target_compile_definitions(
    objclib
    PRIVATE ${LB_OBJCLIB_COMPILE_DEFINITIONS})
target_compile_options(
    objclib
    PRIVATE ${LB_OBJCLIB_COMPILE_OPTIONS})
target_include_directories(
    objclib
    PUBLIC ${LB_OBJCLIB_HEADER_DIR})
target_link_options(
    objclib
    PRIVATE ${LB_OBJCLIB_LINK_OPTIONS})
target_link_libraries(objclib
    PUBLIC clib ${FOUNDATION} objc c++ c)

# TODO: use FRAMEWORK to create ObjCLib.framework
set(LB_LB_OBJCLIB_VERSION "${LB_VERSION_MAJOR}.${LB_VERSION_MINOR}.${LB_VERSION_PATCH}")

set_target_properties(
    objclib PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${LB_LB_OBJCLIB_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER "net.dotslashzero.ObjCLib"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${LB_LB_OBJCLIB_VERSION}
    OUTPUT_NAME "ObjCLib"
    VERSION ${LB_LB_OBJCLIB_VERSION}
    SOVERSION ${LB_LB_OBJCLIB_VERSION}
    PUBLIC_HEADER "${LB_OBJCLIB_HEADER_DIR}/dotslashzero/objclib/ObjCLib.h")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")

# if (UNIX)
#     set(COMMON_FLAGS "-fobjc-arc -W -Wall -Wextra -Werror -pedantic -pedantic-errors")
#     set(C_FLAGS "-std=c11")
#     set(CXX_FLAGS "-std=c++11")
#     set(STATIC_LIBRARY_C_FLAGS "")
#     set(STATIC_LIBRARY_CXX_FLAGS "-fpic -fPIC -fvisibility=hidden")
#     set(EXECUTABLE_C_FLAGS "-fpie -fPIE")
#     set(EXECUTABLE_CXX_FLAGS "")
#     set(LINKER_FLAGS "")
#     set(STATIC_LIBRARY_LINKER_FLAGS "")
#     set(EXECUTABLE_LINKER_FLAGS "")
#     set(SYSTEM_LIBS Foundation objc c++ c)
# elseif (MSVC)
#     message(FATAL_ERROR "MSVC compilers not supported for Objective-C.")
#     return ()
# endif ()

# set(CMAKE_C_FLAGS "${C_FLAGS} ${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")
# set(CMAKE_STATIC_LINKER_FLAGS "${LINKER_FLAGS} ${STATIC_LIBRARY_LINKER_FLAGS}")
# set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
# include_directories(${CMAKE_SOURCE_DIR})

# add_library(objclib STATIC OLApi.h OLApi.mm)
# target_link_libraries(objclib cxxlib clib ${SYSTEM_LIBS})
# set_target_properties(objclib PROPERTIES COMPILE_FLAGS "${STATIC_LIBRARY_CXX_FLAGS}")
# if (APPLE)
#     set_target_properties(objclib PROPERTIES MACOSX_RPATH "${CMAKE_BINARY_DIR}")
# endif ()

# # At the moment, CMake treats .m file as C++ source (which is not correct...), so we have to build manually.
# #add_library(test_objc test/test.m)
# #target_link_libraries(test_objc objclib cxxlib clib ${SYSTEM_LIBS})
# #set_target_properties(test_objc PROPERTIES COMPILE_FLAGS "${EXECUTABLE_C_FLAGS}")
# add_custom_target(
#     test_objc ALL
#     COMMAND ${CMAKE_C_COMPILER} -std=c11 -O3 -fobjc-arc -fpie -fPIE -flto
#         -W -Wall -Wextra -Werror -pedantic -pedantic-errors -I${CMAKE_SOURCE_DIR}
#         -c ${CMAKE_CURRENT_SOURCE_DIR}/test/test.m
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/test.m
#     SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/test.m
#     VERBATIM
# )
# add_custom_command(
#     TARGET test_objc
#     POST_BUILD
#     COMMAND echo "Linking OBJC executable ../test_objc"
#     COMMAND ${CMAKE_C_COMPILER} -flto -o ${CMAKE_BINARY_DIR}/test_objc test.o
#         $<TARGET_FILE:objclib>
#         -framework Foundation -lobjc -L${CMAKE_BINARY_DIR} -lcxxlib -lc++ -lclib -lc
#     VERBATIM
# )
# add_dependencies(test_objc objclib)
