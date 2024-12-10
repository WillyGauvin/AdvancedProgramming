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
#include <condition_variable>
#include <atomic>
#include <chrono>

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
    std::thread sendThread;
    std::thread receiveThread;

    bool active = true;


    void start(const char* ip, unsigned short port)
    {
        IPv4Endpoint endpoint(ip, port);
        connect(endpoint);

        std::string input;

        std::cout << "Enter Name: ";
        std::getline(std::cin, input);

        send(input.c_str(), (int)input.length());

        sendThread = std::thread([this] { this->message_send(); });
        sendThread.detach();

        receiveThread = std::thread([this] {this->message_read(); });
        receiveThread.detach();

        while (active)
        {

        }
    }


    void message_send()
    {
        while (active)
        {
            std::string input;

            std::cout << "You: ";
            std::getline(std::cin, input);

            while (input.length() >= 1024)
            {
                std::cout << "Message Too long. Shorter then 1024 characters." << std::endl;
                std::cout << "You: ";
                std::getline(std::cin, input);
            }

            if (input == "quit")
            {
                active = false;
                break;
            }
            input.push_back('\0');

            send(input.c_str(), (int)input.length());
        }
    }

    void message_read()
    {
        while (active)
        {
            char buffer[1024];
            int bytes = receive(buffer, sizeof(buffer) - 1);
            if (bytes <= 0)
            {
                break;
            }

            buffer[bytes] = '\0';

            std::cout << "\x1b[2K" << "\r";
            std::cout << buffer << std::endl;

            std::cout << "You: ";

        }
    }
};