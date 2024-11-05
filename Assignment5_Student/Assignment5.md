# Assignment: Implementing a Non-blocking Multiplexed Math Client-Server

## Objective

Develop a C++ application that implements a client-server architecture for basic math operations, focusing on **non-blocking I/O**, **multiplexing**, and **single-threaded design**. This assignment will help you understand socket programming concepts and efficient I/O handling in a practical, real-world scenario.

## Background

In network programming, handling multiple clients efficiently without using threads requires careful management of I/O operations. This mirrors real-world scenarios where servers must handle numerous concurrent connections without the overhead of thread management. Using select() for multiplexing allows for efficient handling of multiple clients in a single thread.

## Requirements

### 1. Server Class

Implement a `MathServer` class that handles incoming client connections:

- **Attributes**:
  - `sock` (SOCKET): The server's listening socket
  - `clients` (`std::vector<SOCKET>`): List of connected client sockets

- **Methods**:
  - **Constructor**: Initializes the server socket
  - **`start(const char* ip, unsigned short port)`**: Starts the server on specified IP and port
  - **`bind(IPv4Endpoint endpoint)`**: Binds the server to specified endpoint
  - **`listen()`**: Places socket in listening state
  - **`evaluate(std::string& operation)`**: Parses and evaluates mathematical expressions

### 2. Client Class

Implement a `MathClient` class that connects to the server:

- **Attributes**:
  - `sock` (SOCKET): The client's socket

- **Methods**:
  - **Constructor**: Initializes the client socket
  - **`start(const char* ip, unsigned short port)`**: Connects to server and starts operation
  - **`connect(IPv4Endpoint endpoint)`**: Establishes connection to server
  - **`trySend(const char* data, int length, int& bytesSent)`**: Non-blocking send operation
  - **`tryReceive(char* buffer, int length, int& bytesRead)`**: Non-blocking receive operation

## Instructions

1. **Non-blocking Socket Operations**:
   - Configure all sockets as non-blocking using appropriate socket options
   - Implement proper error handling for would-block conditions

2. **Multiplexing with select()**:
   - Use select() to monitor multiple file descriptors
   - Handle both client connections and data operations in the server
   - Monitor both user input and server responses in the client

3. **Single-threaded Architecture**:
   - Maintain single-threaded design throughout
   - Use non-blocking operations instead of threading for concurrency

4. **Math Operation Handling**:
   - Parse incoming strings containing basic math operations
   - Support operations: addition (+), subtraction (-), multiplication (*), division (/)
   - Return results to appropriate clients
   - 

## Notes
You may use the provided code in class as a starting point for this assignment.  You may not expand the `EchoServer` or `EchoClient` classes, but you may use them as a reference for implementing the `MathServer` and `MathClient` classes.