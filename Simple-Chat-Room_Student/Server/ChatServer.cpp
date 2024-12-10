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
#include <map>
#include <thread>
#include <mutex>

#include "Socket.h"
#include "IPEndpoint.h"
#include "WSASession.h"

#include "Connection.cpp"

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

class ChatServer : public TCPServer
{
public:

    std::thread connectionHandlerThread;
    std::thread clientReceiveThread;

    void start(const char* ip, unsigned short port)
    {
        IPv4Endpoint endpoint(ip, port);
        bind(endpoint);
        listen();

        std::cout << "Server listening on port: " << port << std::endl;

        initiate_chat_room();

        while (true)
        {

        }
    }

    void initiate_chat_room()
    {
        std::cout << "Starting chat room." << std::endl;
        connectionHandlerThread = std::thread([this] {this->ConnectionHandler(); });

        connectionHandlerThread.detach();
    }


private:

    std::map<SOCKET, Connection> connections;
    std::mutex connections_mutex;


    void ConnectionHandler()
    {
        while (true)
        {
            try
            {
                SOCKET client = accept();
                //Create a connection
                Connection connection = Connection(client);

                add_client_to_room(connection);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

    void clientReceive(Connection client) // Receive Thread
    {
        char nameBuffer[1024] = {};
        int nameBytes = recv(client.ClientSocket, nameBuffer, sizeof(nameBuffer), 0);

        client.client_name = nameBuffer;

        std::cout << "Client : " << client.client_name << " has joined!" << std::endl;

        while (true)
        {
            char buffer[1024] = {};
            int bytes;

            bytes = recv(client.ClientSocket, buffer, sizeof(buffer), 0);

            if (bytes <= 0)
            {
                break;
            }

            std::string output = client.client_name + ": " + buffer;

            strncpy_s(buffer, output.c_str(), sizeof(buffer));

            std::cout << buffer << std::endl;

            for (std::map<SOCKET, Connection>::iterator it = connections.begin(); it != connections.end(); it++)
            {
                if (it->first != client.ClientSocket)
                {

                    send(it->first, buffer, sizeof(buffer), 0); // Send what other people have been saying.
                }
            }
        }

        std::unique_lock<std::mutex> removeConnection(connections_mutex);

        closesocket(client.ClientSocket);

        std::string disconnectMessage = client.client_name + " disconnected.";

        std::cout << disconnectMessage << std::endl;

        for (std::map<SOCKET, Connection>::iterator it = connections.begin(); it != connections.end(); it++)
        {
            if (it->first != client.ClientSocket)
            {
                send(it->first, disconnectMessage.c_str(), sizeof(disconnectMessage), 0); // Send what other people have been saying.
            }
        }
        connections.erase(client.ClientSocket);
    }

    void add_client_to_room(Connection c)
    {
        std::unique_lock<std::mutex> removeConnection(connections_mutex);
        connections.insert({ c.ClientSocket, c });

        clientReceiveThread = std::thread([this, c] {this->clientReceive(c); });
        clientReceiveThread.detach();
    }

};
