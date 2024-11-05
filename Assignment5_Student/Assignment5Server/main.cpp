#include "Assignment5Server.cpp"

int main() {
   try {
      WSASession session; // Initialize WinSock
      MathServer server; // Create a MathServer
      server.start("127.0.0.1", 8080); // Start the server on any address, port 8080
   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }
   return 0;
}