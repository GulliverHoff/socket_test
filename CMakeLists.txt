cmake_minimum_required(VERSION 3.16)
project(IP_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(utils)

add_executable(IP_demo ${SOURCES})

if(WIN32)
    target_compile_definitions(IP_demo PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(IP_demo PRIVATE ws2_32 iphlpapi)
endif()




