#include "ip_utils.hpp"
#include <iostream>
#include <string>
#include <cstring>

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <iphlpapi.h>
  #pragma comment(lib, "ws2_32.lib")
  #pragma comment(lib, "iphlpapi.lib")
#else
  #include <ifaddrs.h>
  #include <arpa/inet.h>
  #include <netinet/in.h>
#endif

void printLocalIP() {
#ifdef _WIN32
    ULONG bufLen = 15000;
    auto* addrs = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);
    DWORD result = GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER, nullptr, addrs, &bufLen);

    if (result == ERROR_BUFFER_OVERFLOW) {
        free(addrs);
        addrs = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);
        result = GetAdaptersAddresses(AF_INET, 0, nullptr, addrs, &bufLen);
    }

    if (result == NO_ERROR) {
        for (auto* adapter = addrs; adapter; adapter = adapter->Next) {
            if (adapter->OperStatus != IfOperStatusUp) continue;
            for (auto* addr = adapter->FirstUnicastAddress; addr; addr = addr->Next) {
                sockaddr_in* sa = (sockaddr_in*)addr->Address.lpSockaddr;
                char ipStr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sa->sin_addr), ipStr, sizeof(ipStr));
                if (strncmp(ipStr, "127.", 4) != 0) {
                    std::wcout << L"Local IP (" << adapter->FriendlyName << L"): " << ipStr << std::endl;
                }
            }
        }
    } else {
        std::cerr << "GetAdaptersAddresses failed.\n";
    }
    free(addrs);
#else
    struct ifaddrs* ifaddr;
    getifaddrs(&ifaddr);
    for (auto* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
        void* addr_ptr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, addr_ptr, ip, sizeof(ip));
        if (strncmp(ip, "127.", 4) != 0) {
            std::cout << "Local IP (" << ifa->ifa_name << "): " << ip << std::endl;
        }
    }
    freeifaddrs(ifaddr);
#endif
}
