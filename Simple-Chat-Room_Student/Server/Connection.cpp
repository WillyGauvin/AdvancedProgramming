
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



class Connection
{
public:
	SOCKET ClientSocket;
	std::string client_name = "";

	Connection(const SOCKET ClientSocket)
		: ClientSocket(ClientSocket)
	{
	}
};