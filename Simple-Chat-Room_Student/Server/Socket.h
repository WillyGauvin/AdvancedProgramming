#pragma once

// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <stdexcept>
#include <string>

class Socket {
protected:
   SOCKET sock; // WinSock socket handle

   Socket(int af, int type, int protocol) : sock(INVALID_SOCKET) {
      sock = socket(af, type, protocol); // Create the socket
      if (sock == INVALID_SOCKET) { // Check for errors
         throw std::runtime_error("Socket creation failed: " +
            std::to_string(WSAGetLastError())); // Throw an exception
      }
   }

public:
   ~Socket() {
      if (sock != INVALID_SOCKET) { // Check if the socket is valid
         closesocket(sock); // Close the socket
      }
   }
   // Prevent copying
   Socket(const Socket&) = delete;
   Socket& operator=(const Socket&) = delete;
};

class NonBlockingSocket : public Socket {
protected:
   void setNonBlocking(bool nonBlocking = true) { // Function to set non-blocking mode
      unsigned long mode = nonBlocking ? 1 : 0; // 1 for non-blocking, 0 for blocking
      if (ioctlsocket(sock, FIONBIO, &mode) == SOCKET_ERROR) { // Set non-blocking mode
         throw std::runtime_error("Failed to set non-blocking mode: " +
            std::to_string(WSAGetLastError()));
      }
   }

   bool wouldBlock() { // Check if operation would block
      return WSAGetLastError() == WSAEWOULDBLOCK;
   }

public:
   NonBlockingSocket(int af, int type, int protocol) : Socket(af, type, protocol) {
      setNonBlocking(); // Set non-blocking mode
   }
};

class TCPSocket : public Socket {
public:
   TCPSocket() : Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP) {} // TCP socket
};

class NonBlockingTCPSocket : public NonBlockingSocket {
public:
   NonBlockingTCPSocket() : NonBlockingSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP) {} // TCP socket
};

class UDPSocket : public Socket {
public:
   UDPSocket() : Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) {} // UDP socket
};

class NonBlockingUDPSocket : public NonBlockingSocket {
public:
   NonBlockingUDPSocket() : NonBlockingSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) {} // UDP socket
};