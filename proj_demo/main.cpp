#include "ip_utils.hpp"

#ifdef _WIN32
  #include "iostream"
  #include <winsock2.h>
#endif

int main() {
    printLocalIP();
}
