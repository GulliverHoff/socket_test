cmake_minimum_required(VERSION 3.16)
project(IP_TCP_demo_stage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/executables)

add_subdirectory(utils)
add_subdirectory(host)
add_subdirectory(client)

add_subdirectory(proj_demo)
