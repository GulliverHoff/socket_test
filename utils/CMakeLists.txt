#Make ip_utils library
file(GLOB IP_UTILS_SOURCES src/*.cpp)

add_library(ip_utils ${IP_UTILS_SOURCES})
target_include_directories(ip_utils PUBLIC include)
