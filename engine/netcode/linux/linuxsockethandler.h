#pragma once
#include "netcode/constants.h"
#include "sys/socket.h"
#include "sys/types.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include <string>

namespace Netcode
{
	struct RCState
	{
		unsigned char RC4_STATE[256];
		int i;
		int j;

		RCState()
		{
			i = 0;
			j = 0;
		}
	};

	class SocketHandler
	{
	public:
		SocketHandler();
		SocketHandler(const int sock);
		~SocketHandler();

		///Starts the socket
		bool initSocket(std::string ipaddress);

		///Listens for connections
		bool socketListen();

		///Closes the socket
		void closesocket();
		bool sendData(const char* buffer, size_t size);
		char* recieveData();
		int& getSocket();
		int getBufLength();

		void InitRC4();
		std::string CryptRC4(RCState& state, const char* in, const int& length);

		RCState RC4_SEND;
		RCState RC4_RECIEVE;

	private:
		int mSocket;
		char recvbuf[DEFAULT_BUFLEN];
		int bufLength;

		
		bool RC4ENABLED = false;
		const int RC4KEYLENGTH = 5;
		const char* RC4KEY;
		
		void swap(unsigned char *s, unsigned int i, unsigned int j);

		//structure contains information about the Windows Sockets implementation.
		//WSADATA wsaData;
		int iResult;

	};


	
}
