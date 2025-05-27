file(GLOB CLIENT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(TCP_Client ${CLIENT_SOURCES})

target_include_directories(TCP_Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(TCP_Client PRIVATE ip_utils)

if(WIN32)
    target_compile_definitions(TCP_Client PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(TCP_Client PRIVATE ws2_32 iphlpapi)
endif()
