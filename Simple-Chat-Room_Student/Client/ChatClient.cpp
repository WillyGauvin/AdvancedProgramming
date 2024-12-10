// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <iostream>
#include <string>

#include <thread>
#include <mutex>

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

        std::cout << "Enter Name: ";
        std::getline(std::cin, input);

        send(input.c_str(), input.length());

        std::thread send(message_send);
        send.detach();

        std::thread receive(message_read);
        receive.detach();
    }

private:

    void message_send()
    {
        std::string input;
        char buffer[1024];

        while (true)
        {
            std::cout << "You: ";
            std::getline(std::cin, input);
            if (input == "quit") break;

            send(input.c_str(), input.length());
        }
    }

    void message_read()
    {
        std::string input;
        char buffer[1024];

        while (true)
        {
            int bytes = receive(buffer, sizeof(buffer) - 1);
            buffer[bytes] = '\0';

            std::cout << buffer << std::endl;
        }
    }
};