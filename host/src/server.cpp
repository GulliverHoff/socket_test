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
    server_addr.sin_port = htons(31415);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed.\n";
        return;
    }

    if (listen(server_socket, 1) < 0) {
        std::cerr << "Listen failed.\n";
        return;
    }

    std::cout << "Server listening on port 31415...\n";

    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        return;
    }

    std::cout << "Client connected. Type integers to send:\n";

    while (true) {
        int32_t number;
        std::cout << "> ";
        if (!(std::cin >> number)) {
            std::cerr << "Invalid input. Exiting.\n";
            break;
        }

        int32_t net_number = htonl(number);
        int sent = send(client_socket, (char*)&net_number, sizeof(net_number), 0);
        if (sent <= 0) {
            std::cerr << "Send failed or client disconnected.\n";
            break;
        }
    }

    CLOSESOCKET(client_socket);
    CLOSESOCKET(server_socket);
}
