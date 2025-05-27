#include "ip_utils.hpp"
#include "client.hpp"

#include <iostream>

#ifdef _WIN32
  #include <winsock2.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }
#endif

    const char* ip = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = 31415;

    std::cout << "Connecting to " << ip << ":" << port << "...\n";
    run_client(ip, port);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
