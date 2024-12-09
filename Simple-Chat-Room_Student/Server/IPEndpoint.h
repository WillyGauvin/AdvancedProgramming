#pragma once

// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <stdexcept>

class IPv4Endpoint {
   sockaddr_in addr; // IPv4 address structure (sockaddr_in)
public:
   IPv4Endpoint(const char* ip, unsigned short port) {
      addr.sin_family = AF_INET; // IPv4
      addr.sin_port = htons(port); // Host to network short

      if (ip == nullptr) {
         addr.sin_addr.s_addr = INADDR_ANY; // Bind to any address
      }
      else if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
         throw std::runtime_error("Invalid IP address");
      }
   }

   const sockaddr* as_sockaddr() const {
      return reinterpret_cast<const sockaddr*>(&addr); // Convert to sockaddr
   }
   int size() const { return sizeof(addr); } // Size of the address structure
};
