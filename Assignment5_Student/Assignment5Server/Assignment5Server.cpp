// Initialize WinSock2
#define WIN32_LEAN_AND_MEAN // Reduce Windows header bloat
#include <winsock2.h>       // Core WinSock functionality
#include <ws2tcpip.h>       // TCP/IP specific functions

#pragma comment(lib, "Ws2_32.lib") // Link with Ws2_32.lib

// Note: winsock2.h must come before windows.h if used

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Socket.h"
#include "IPEndpoint.h"
#include "WSASession.h"

// TCP Server
/*
* -Singly threaded
* -Blocking
*/
class TCPServer : public TCPSocket {
protected:
   // Bind to address
   void bind(const IPv4Endpoint& endpoint) { // Bind to an endpoint
      if (::bind(sock, endpoint.as_sockaddr(), endpoint.size())
         == SOCKET_ERROR) {
         throw std::runtime_error("Bind failed: " +
            std::to_string(WSAGetLastError()));
      }
   }

   // Listen for connections
   void listen(int backlog = SOMAXCONN) { // Listen for connections
      if (::listen(sock, backlog) == SOCKET_ERROR) {
         throw std::runtime_error("Listen failed: " +
            std::to_string(WSAGetLastError()));
      }
   }

   // Accept client connection
   SOCKET accept() { // Accept a client connection
      SOCKET client = ::accept(sock, nullptr, nullptr);
      if (client == INVALID_SOCKET) {
         throw std::runtime_error("Accept failed: " +
            std::to_string(WSAGetLastError()));
      }
      return client;
   }
};

// Non-blocking TCP Server
/*
* -Singly threaded
* -Non-blocking
*/
class NonBlockingTCPServer : public NonBlockingTCPSocket {
protected:
   // Bind to address
   void bind(const IPv4Endpoint& endpoint) { // Bind to an endpoint
      if (::bind(sock, endpoint.as_sockaddr(), endpoint.size())
         == SOCKET_ERROR) {
         throw std::runtime_error("Bind failed: " +
            std::to_string(WSAGetLastError()));
      }
   }

   // Listen for connections
   void listen(int backlog = SOMAXCONN) { // Listen for connections
      if (::listen(sock, backlog) == SOCKET_ERROR) {
         throw std::runtime_error("Listen failed: " +
            std::to_string(WSAGetLastError()));
      }
   }

   // Accept client connection
   SOCKET accept() { // Accept a client connection
      SOCKET client = ::accept(sock, nullptr, nullptr);
      if (client == INVALID_SOCKET) {
         if (!wouldBlock()) {
            throw std::runtime_error("Accept failed: " +
               std::to_string(WSAGetLastError()));
         }
      }
      return client;
   }
};

// Math Server
/*
* -Singly threaded
* -Non-blocking
* -Multiplexing
*/
// This server receives a math operation from the client as a string and sends the result back
// This server can handle at least two clients at the same time
// The server should be able to handle the following operations:
// - Addition
// - Subtraction
// - Multiplication
// - Division
// 
// IMPLEMENT HERE

class MathServer : public NonBlockingTCPServer
{
public:
    void start(const char* ip, unsigned short port)
    {
        IPv4Endpoint endpoint(ip, port);
        bind(endpoint);
        listen();

        std::cout << "Server listening on port: " << port << std::endl;

        std::vector<SOCKET> clients; //Vector of clients

        while (true)
        {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sock, &readfds);

            int nfds = static_cast<int>(sock);
            for (SOCKET client : clients)
            {
                FD_SET(client, &readfds);
                if (static_cast<int>(client) > nfds)
                {
                    nfds = static_cast<int>(client);
                }
            }

            if (select(nfds + 1, &readfds, nullptr, nullptr, nullptr) == SOCKET_ERROR)
            {
                throw std::runtime_error("Select failed: " + std::to_string(WSAGetLastError()));
            }

            if (FD_ISSET(sock, &readfds)) 
            {
                try 
                {
                    SOCKET client = accept();
                    clients.push_back(client);
                    std::cout << "New client connected. Total clients: " << clients.size() << std::endl;
                }
                catch (const std::exception& e) 
                {
                    std::cerr << "Accept failed: " << e.what() << std::endl;
                }
            }

            for (auto it = clients.begin(); it != clients.end(); ) 
            {
                SOCKET client = *it;
                if (FD_ISSET(client, &readfds))
                {
                    char buffer[1024];
                    int bytes = recv(client, buffer, sizeof(buffer), 0);

                    if (bytes <= 0) 
                    {
                        std::cout << "Client disconnected" << std::endl;
                        closesocket(client);
                        it = clients.erase(it);
                        continue;
                    }

                    std::string str = Operations(buffer);
                    char output[1024];
                    strcpy_s(output, str.c_str());

                    if (send(client, output, (int)str.length(), 0) == SOCKET_ERROR)
                    {
                        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                        closesocket(client);
                        it = clients.erase(it);
                        continue;
                    }
                }
                ++it;
            }
        }
    }

    std::string Operations(const std::string& expression)
    {
        std::vector<float> numbers;
        char op = ' ';

        std::stringstream ss(expression);

        std::string token;
        while (getline(ss, token, ' '))
        {
            if (token.empty())
            {
                continue;
            }
            if (isdigit(token[0]))
            {
                float num;
                std::stringstream(token) >> num; //Convert to float
                numbers.push_back(num);
            }
            else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/')
            {
                op = token[0];
            }
        }

        if (numbers.size() != 2)
        {
            return "Invalid expression " + std::to_string(numbers.size());
        }


        std::string result;

        float floatresult;

        switch (op)
        {
        case '+':
            floatresult = numbers[0] + numbers[1];
            result = std::to_string(floatresult);
            break;
        case '-':
            floatresult = numbers[0] - numbers[1];
            result = std::to_string(floatresult);

            break;
        case '*':
            floatresult = numbers[0] * numbers[1];
            result = std::to_string(floatresult);

            break;
        case '/':
            if (numbers[1] == 0)
            {
                result = "Error, division by 0";
                break;
            }
            floatresult = numbers[0] / numbers[1];
            result = std::to_string(floatresult);

            break;
        default:
            result = "Operator not recognized";
            break;
        }
        return result;

    }
    
};