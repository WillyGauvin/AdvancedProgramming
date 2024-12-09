+-----------------------------------------------------+
|                     Connection                      |
+-----------------------------------------------------+
| + ClientSocket: SOCKET                              |
| + ClientSocketAddress: sockaddr_in                  |
| + client_name: std::string                          |
+-----------------------------------------------------+
| + Connection(ClientSocket: const SOCKET,            |
|             ClientSocketAddress: const sockaddr_in) |
+-----------------------------------------------------+

+------------------------------------------------+
|                 Server_Socket                  |
+------------------------------------------------+
| - wsaData: WSADATA                             |
| - ServerSocket: SOCKET                         |
| - ServerSocketAddress: sockaddr_in             |
| - connections: std::map<SOCKET, Connection>    |
| - connections_mutex: std::mutex                |
+------------------------------------------------+
| + Server_Socket(RequestVersion: int)           |
| + ~Server_Socket()                             |
| + start_server(PortNumber: int): void          |
| + initiate_chat_room(): void                   |
| - SetServerSockAddr(sockAddr: sockaddr_in*,    |
|                    PortNumber: int): void      |
| - read_message(c: Connection): void            | 
| - write_message(): void                        |
| - add_client_to_room(c: Connection&): void     |
+------------------------------------------------+

+--------------------------------------------------+
|                  Client_Socket                   |
+--------------------------------------------------+
| - wsaData: WSADATA                               |
| - ClientSocket: SOCKET                           |
| - ClientSocketAddress: sockaddr_in               |
+--------------------------------------------------+
| + Client_Socket(RequestVersion: int)             |
| + ~Client_Socket()                               |
| + ConnectServer(server_ip: const char*,          |
|                server_port: const int): void     |
| + initiate_chat_room(): void                     |
| - SetClientSockAddr(server_ip: const char*,      |
|                    server_port: const int): void |
| - message_send(): void                           |
| - message_read(): void                           |
+--------------------------------------------------+