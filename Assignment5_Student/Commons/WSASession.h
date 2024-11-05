#pragma once

// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <stdexcept>
#include <string>

struct WSASession {
   WSASession() {
      WSADATA wsaData; // WinSock data structure
      int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Initialize WinSock
      if (result != 0) {
         throw std::runtime_error("WSAStartup failed: " +
            std::to_string(result));
      }
   }
   ~WSASession() { WSACleanup(); } // Clean up WinSock
   WSASession(const WSASession&) = delete;
   WSASession& operator=(const WSASession&) = delete;
};