#include "client.hpp"
#include "ip_utils.hpp"

#include <iostream>
#include <cstring>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #define CLOSESOCKET closesocket
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #define CLOSESOCKET close
  using SOCKET = int;
  #define INVALID_SOCKET -1
#endif

void run_client(const char* server_ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed to " << server_ip << ":" << port << "\n";
        CLOSESOCKET(sock);
        return;
    }

    std::cout << "Connected to server. Waiting for integers...\n";

    while (true) {
        int32_t number;
        int received = recv(sock, (char*)&number, sizeof(number), 0);
        if (received <= 0) {
            std::cerr << "Server closed connection or error occurred.\n";
            break;
        } else if (received == sizeof(number)) {
            number = ntohl(number);
            std::cout << "Received: " << number << std::endl;
        }
    }

    CLOSESOCKET(sock);
}
