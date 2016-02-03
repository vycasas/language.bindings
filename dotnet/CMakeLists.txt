cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(dotnetlib CXX)

if (NOT MSVC)
    message(FATAL_ERROR "Only MSVC compilers are supported for .NET.")
    return ()
endif ()

set(CXX_FLAGS "/clr")
set(SHARED_LIBRARY_CXX_FLAGS "")
set(EXECUTABLE_CXX_FLAGS "")
set(LINKER_FLAGS "")
set(SHARED_LIBRARY_LINKER_FLAGS "")
set(EXECUTABLE_LINKER_FLAGS "")
set(SYSTEM_LIBS)

set(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${LINKER_FLAGS} ${SHARED_LIBRARY_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS} ${EXECUTABLE_LINKER_FLAGS}")
include_directories(${CMAKE_SOURCE_DIR})

add_library(dotnetlib SHARED api_dotnet.cxx api_dotnet.hxx resource.hxx assemblyinfo.cxx app.ico app.rc)
target_link_libraries(dotnetlib cxxlib clib)
set_target_properties(dotnetlib PROPERTIES OUTPUT_NAME "DotNetLib")
set_target_properties(dotnetlib PROPERTIES COMPILE_FLAGS "${SHARED_LIBRARY_CXX_FLAGS}")
set_target_properties(dotnetlib PROPERTIES VS_DOTNET_REFERECES "System")

string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# C# and VB tests are not included. They can be ran separately.
add_executable(
    test_dotnet
    test/test_dotnet_cxxcli/program.cxx
    test/test_dotnet_cxxcli/assemblyinfo.cxx
    test/test_dotnet_cxxcli/resource.hxx
    test/test_dotnet_cxxcli/app.ico
    test/test_dotnet_cxxcli/app.rc
)
target_link_libraries(test_dotnet dotnetlib cxxlib clib ${SYSTEM_LIBS})
set_target_properties(test_dotnet PROPERTIES COMPILE_FLAGS "${EXECUTABLE_CXX_FLAGS}")
set_target_properties(dotnetlib PROPERTIES VS_DOTNET_REFERECES "$<TARGET_FILE:dotnetlib>;System")

string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
