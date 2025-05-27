#include "ip_utils.hpp"
#include "server.hpp"

#ifdef _WIN32
  #include <winsock2.h>
#endif

int main() {

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }
#endif

    printLocalIP();

    //run_server();

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
