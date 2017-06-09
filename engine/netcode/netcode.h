#pragma once
#pragma comment(lib,"Ws2_32.lib")

#define WIN32_LEAN_AND_MEAN
#include "netdata.h"
#include <winsock2.h>
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
		~SocketHandler();

		bool initSocket(const char* ip);
		void close();
		bool sendData(const char* buffer, size_t size);
		char* recieveData();
		SOCKET getSocket();
		int getBufLength();

		void InitRC4();
		std::string CryptRC4(RCState& state, const char* in, const int& length);

		RCState RC4_SEND;
		RCState RC4_RECIEVE;

	private:
		SOCKET ConnectSocket;
		char recvbuf[DEFAULT_BUFLEN];
		int bufLength;
		bool RC4ENABLED = false;
		const int RC4KEYLENGTH = 5;
		const char* RC4KEY;

		void swap(unsigned char *s, unsigned int i, unsigned int j);

		//structure contains information about the Windows Sockets implementation.
		WSADATA wsaData;
		int iResult;

	};
}