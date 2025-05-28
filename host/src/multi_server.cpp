#include "server.hpp"
#include "ip_utils.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstring>

#ifdef _WIN32
  #include <winsock2.h>
  #undef max
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


namespace {
    std::vector<SOCKET> clients;
    std::mutex clients_mutex;
    std::atomic<bool> running{true};

    void client_thread(SOCKET client_socket, std::string client_ip) {
        std::cout << "-- Client thread started for " << client_ip << "\n>";

        // Wait until client disconnects (you could read messages here if needed)
        char buffer[1];
        while (recv(client_socket, buffer, 1, MSG_PEEK) > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "-- Client " << client_ip << " disconnected.\n>";

        // Remove from clients list
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
        CLOSESOCKET(client_socket);
    }

    void broadcast_integer(int32_t number) {
        int32_t net_number = htonl(number);
        std::lock_guard<std::mutex> lock(clients_mutex);

        for (auto it = clients.begin(); it != clients.end(); ) {
            if (send(*it, (char*)&net_number, sizeof(net_number), 0) <= 0) {
                std::cerr << "-- Failed to send to a client. Removing.\n";
                CLOSESOCKET(*it);
                it = clients.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void run_server() {
    printLocalIP();

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "-- Socket creation failed.\n";
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(31415);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "-- Bind failed.\n";
        return;
    }

    if (listen(server_socket, SOMAXCONN) < 0) {
        std::cerr << "-- Listen failed.\n";
        return;
    }

    std::cout << "Server listening on port 31415...\n";

    // Accept clients in a separate thread
    std::thread accept_thread([&]() {
        while (running) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
            if (client_socket == INVALID_SOCKET) continue;

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            std::cout << "-- Client connected from IP: " << client_ip << "\n>";

            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(client_socket);
            }

            std::thread(client_thread, client_socket, std::string(client_ip)).detach();
        }
    });

    // Main input loop for sending integers
    std::cout << "Type integers to broadcast (Ctrl+C to stop):\n";
    while (running) {
        int32_t number;
        std::cout << "> ";
        if (!(std::cin >> number)) {
            std::cerr << "-- Invalid input ignored.\n";
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        broadcast_integer(number);
    }

    running = false;
    CLOSESOCKET(server_socket);
    accept_thread.join();

    // Clean up remaining clients
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (SOCKET sock : clients) {
        CLOSESOCKET(sock);
    }
    clients.clear();
}   
