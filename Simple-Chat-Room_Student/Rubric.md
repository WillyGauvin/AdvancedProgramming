### Simple Rubric for C++ Socket Programming Project

#### Objective
Implement a multithreaded chat server and client system using blocking sockets, adhering to the provided process flow diagram. Your implementation should leverage threads to handle blocking operations and simulate a chat room.

---

#### Criteria & Grading Breakdown:
1. **Socket Initialization and Setup (6.25 points)**
   - [ ] Correct initialization of Winsock (WSAStartup) for both client and server (1.5 points)
   - [ ] Proper socket creation and binding (Server) (1.5 points)
   - [ ] Proper socket creation and connection setup (Client) (1.5 points)
   - [ ] Graceful shutdown and cleanup of sockets (WSACleanup, closesocket) (1.75 points)

2. **Thread Management (9.25 points)**
   - [ ] **Server:** Spawns a thread for each client connection using a `Connection` class (2.5 points)
   - [ ] **Client:** Spawns separate threads for sending (`message_send`) and receiving (`message_read`) messages (2.5 points)
   - [ ] Ensures threads are detached or properly joined to avoid resource leaks (2 points)
   - [ ] Handles race conditions or synchronization issues effectively (e.g., using `std::mutex` for shared resource access) (2.25 points)

3. **Message Handling (9.25 points)**
   - [ ] Correctly sends and receives messages between clients and the server (2.5 points)
   - [ ] Broadcast functionality implemented on the server to forward messages to all clients except the sender (2.5 points)
   - [ ] Proper handling of message size and edge cases (2 points)
   - [ ] Disconnection handling (2.25 points)

4. **Synchronization of Shared Resources (3.5 points)**
   - [ ] Use of `std::mutex` or equivalent to synchronize access to shared resources (e.g., `connections` map) (1.75 points)
   - [ ] Avoids data races when multiple threads access or modify shared data (1.75 points)

5. **Exception Handling and Resilience (3.5 points)**
   - [ ] Graceful handling of exceptions during socket operations (e.g., connection failures, message send/receive errors) (1.75 points)
   - [ ] Provides meaningful error messages for debugging and recovery (1 point)
   - [ ] Recovers from errors without crashing the program (0.75 points)

6. **Adherence to Process Flow Diagram (5.75 points)**
   - [ ] Process flow aligns closely with the sequence and state transitions defined in the diagram (3 points)
   - [ ] Logical consistency between client-server connection, message handling, and thread management (2.75 points)

Total: 37.5 points

---

### Setup

Please use the included project structure and files to implement the chat server and client system. Additionally, ensure that your implementation adheres to the provided process flow diagram and the requirements outlined in the rubric.  You may use any previously provided materials or resources to assist in your implementation however it is worth noting that you are to implement the chat server and client system using blocking sockets using threads to handle blocking operations.

---

### Submission:
- Submit all relevant files (`Client.h`, `Client.cpp`, `Server.h`, `Server.cpp`, and `main.cpp`).
- Ensure that your implementation compiles and runs **without errors**.
- Ensure that your implementation compiles and runs **without warnings**.
- Naming conventions and coding style should be consistent but they are up to you.