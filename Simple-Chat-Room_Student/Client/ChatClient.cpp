// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <iostream>
#include <string>

#include "Socket.h"
#include "IPEndpoint.h"
#include "WSASession.h"

// TCP Client
/*
* -Singly threaded
* -Blocking
*/
class TCPClient : public TCPSocket {
protected:
    void connect(const IPv4Endpoint& endpoint) { // Connect to an endpoint
        if (::connect(sock, endpoint.as_sockaddr(), endpoint.size())
            == SOCKET_ERROR) {
            throw std::runtime_error("Connect failed: " +
                std::to_string(WSAGetLastError()));
        }
    }

    int send(const char* data, int length) { // Send data
        int bytes = ::send(sock, data, length, 0);
        if (bytes == SOCKET_ERROR) {
            throw std::runtime_error("Send failed: " +
                std::to_string(WSAGetLastError()));
        }
        return bytes;
    }

    int receive(char* buffer, int length) { // Receive data
        int bytes = recv(sock, buffer, length, 0);
        if (bytes == SOCKET_ERROR) {
            throw std::runtime_error("Receive failed: " +
                std::to_string(WSAGetLastError()));
        }
        return bytes;
    }
};

// TCP Client
/*
* -Singly threaded
* -Non-blocking
*/
class NonBlockingTCPClient : public NonBlockingTCPSocket {
protected:
    void connect(const IPv4Endpoint& endpoint) { // Connect to an endpoint
        if (::connect(sock, endpoint.as_sockaddr(), endpoint.size())
            == SOCKET_ERROR) {
            if (!wouldBlock()) { // Check if the operation would block
                throw std::runtime_error("Connect failed: " +
                    std::to_string(WSAGetLastError()));
            }
        }
    }

    bool trySend(const char* data, int length, int& bytesSent) { // Try to send data
        bytesSent = ::send(sock, data, length, 0);
        if (bytesSent == SOCKET_ERROR) {
            if (!wouldBlock()) { // Check if the operation would block
                throw std::runtime_error("Send failed: " +
                    std::to_string(WSAGetLastError()));
            }
            return false;
        }
        return true;
    }

    bool tryReceive(char* buffer, int length, int& bytesReceived) { // Try to receive data
        bytesReceived = recv(sock, buffer, length, 0);
        if (bytesReceived == SOCKET_ERROR) {
            if (!wouldBlock()) { // Check if the operation would block
                throw std::runtime_error("Receive failed: " +
                    std::to_string(WSAGetLastError()));
            }
            return false;
        }
        return true;
    }
};

// Math Client
/*
* -Singly threaded
* -Non-blocking
* -Multiplexing
*/
// This client will connect to the MathServer and send a math problem to solve
// This client will then receive the result from the MathServer
// This client WILL NOT receive any other client's math problems or results
// IMPLEMENT HERE

class ChatClient : public TCPClient
{
public:
    void start(const char* ip, unsigned short port)
    {
        IPv4Endpoint endpoint(ip, port);
        connect(endpoint);

        std::string input;
        char buffer[1024];
        
        while (true)
        {
            std::cout << "Enter message: ";
            std::getline(std::cin, input);

            if (input == "quit") break;

            send(input.c_str(), input.length());

            int bytes = receive(buffer, sizeof(buffer) - 1);
            buffer[bytes] = '\0';

            std::cout << "Server echo: " << buffer << std::endl;
        }
    }
};