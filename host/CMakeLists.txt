#file(GLOB HOST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
set(HOST_SOURCES 
    src/main.cpp
    src/server.cpp
)

add_executable(TCP_Host ${HOST_SOURCES})

target_include_directories(TCP_Host PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TCP_Host PRIVATE ip_utils)

if(WIN32)
    target_compile_definitions(TCP_Host PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(TCP_Host PRIVATE ws2_32 iphlpapi)
endif()
