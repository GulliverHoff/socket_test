add_executable(IP_demo main.cpp)
target_link_libraries(IP_demo PRIVATE ip_utils)

if(WIN32)
    target_compile_definitions(IP_demo PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(IP_demo PRIVATE ws2_32 iphlpapi)
endif()
