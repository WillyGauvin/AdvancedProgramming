#include "Assignment5Client.cpp"

int main() {
   try {
      WSASession session; // Initialize WinSock
      MathClient client; // Create a MathClient
      client.start("127.0.0.1", 8080); // Connect to localhost, port 8080
   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }
   return 0;
}