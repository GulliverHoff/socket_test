cmake_minimum_required(VERSION 3.16)
project(TCP_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

add_executable(TCP_server_Demo ${SOURCES})

if(WIN32)
    target_compile_definitions(TCP_server_Demo PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(TCP_server_Demo PRIVATE ws2_32 iphlpapi)
endif()




