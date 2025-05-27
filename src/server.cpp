#include "server.hpp"
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

void run_server() {
    printLocalIP();

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed.\n";
        return;
    }

    if (listen(server_socket, 1) < 0) {
        std::cerr << "Listen failed.\n";
        return;
    }

    std::cout << "Server listening on port 12345...\n";

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        return;
    }

    char buffer[1024] = {};
    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    } else {
        std::cerr << "Receive failed.\n";
    }

    CLOSESOCKET(client_socket);
    CLOSESOCKET(server_socket);
}
