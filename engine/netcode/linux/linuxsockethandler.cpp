#include "config.h"
#include "linuxsockethandler.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <thread>

namespace Netcode
{
	SocketHandler::SocketHandler()
	{
		RC4KEY = "banan";
		mSocket = -1;
		bufLength = 0;
	}

	SocketHandler::SocketHandler(const int sock)
	{
		RC4KEY = "banan";
		this->mSocket = sock;
		bufLength = 0;
	}

	SocketHandler::~SocketHandler()
	{
	    this->close();
	}

	bool SocketHandler::initSocket()
	{		
		int n;
		mSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(mSocket < 0)
		{
		  _printf("Could not create socket!");
		  return false;
		}
		
		_printf("Socket Initialized!");

		//Socket is now initialized
		return true;
	}

	bool SocketHandler::socketConnect(const std::string& ipaddress)
	{
		struct sockaddr_in serv_addr;
		bzero((char*) &serv_addr, sizeof(serv_addr));
		
		//std::string sargv = "192.168.1.72";
		const char* argv = ipaddress.c_str();

		_printf("Connecting to %s:%s", argv, DEFAULT_PORT);
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(argv);
		serv_addr.sin_port = htons(atoi(DEFAULT_PORT));

		if (connect(this->mSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		{
			_error("ERROR connecting");
			return false;
		}

		_printf("Connected!");
		return true;
	}

	bool SocketHandler::socketListen(const std::string& ipaddress)
	{
		struct sockaddr_in serv_addr;
		bzero((char*) &serv_addr, sizeof(serv_addr));
		
		//std::string sargv = "192.168.1.72";
		const char* argv = ipaddress.c_str();

		_printf("Address is %s:%s", argv, DEFAULT_PORT);
		
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(argv);
		serv_addr.sin_port = htons(atoi(DEFAULT_PORT));

		// set SO_REUSEADDR on a socket to true (1):
		int optval = 1;
		setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

		if(bind(mSocket, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		{
		  _printf("Could not bind socket!");
		  return false;
		}
		
		listen(mSocket, SOMAXCONN);		
		return true;
	}

	void SocketHandler::close()
	{
		//Shutdown the socket!
		if(this->mSocket != -1)
		{
			_printf("Thread %ld: Closed socket %i", std::this_thread::get_id() ,this->mSocket);	
			::close(this->mSocket);
		}
	}


	bool SocketHandler::sendData(const char* sendbuf, size_t size)
	{
		char* buf = new char[size];
		if (RC4ENABLED)
		{
			std::string str = CryptRC4(RC4_SEND, sendbuf, size);
			memcpy((void*)buf, (void*)str.c_str(), size);
		}
		else
		{
			memcpy((void*)buf, (void*)sendbuf, size);
		}

		// Send a buffer
		if (send(this->mSocket, buf, size, 0) == -1)
		{
			_printf("Send failed!");
			delete[] buf;
			::close(this->mSocket);
			return false;
		}
		
		_printf("Bytes Sent: %i", iResult);
		
		delete[] buf;
		
		return true;
	}

	char* SocketHandler::read()
	{
		int recvbuflen = DEFAULT_BUFLEN;
		
		int iResult = recv(this->mSocket, this->recvbuf, recvbuflen, 0);
		
		if (iResult > 0)
		{
			_printf("Bytes received: %d", iResult);
			this->bufLength = iResult;

			if (RC4ENABLED)
			{
				std::string str = CryptRC4(RC4_RECIEVE, recvbuf, bufLength);
				memcpy((void*)recvbuf, (void*)str.c_str(), bufLength);
			}

			return this->recvbuf;
		}
		else if(iResult == -1)
		{
			_printf("Connection closed.");
			//this->closesocket();
		}
	
		return nullptr;
	
	}

	const int& SocketHandler::getSocket() const
	{
		return this->mSocket;
	}

	int SocketHandler::bytesAvailable()
	{
		return this->bufLength;
	}

	void SocketHandler::swap(unsigned char *s, unsigned int i, unsigned int j)
	{
		s[i] ^= s[j];
		s[j] ^= s[i];
		s[i] ^= s[j];
	}

	void SocketHandler::InitRC4()
	{
		RC4_SEND = RCState();
		RC4_RECIEVE = RCState();

		for (int i = 0; i < 256; i++)
		{
			RC4_SEND.RC4_STATE[i] = i;
		}

		unsigned char j = 0;
		for (int i = 0; i < 256; i++)
		{
			j = (j + RC4_SEND.RC4_STATE[i] + RC4KEY[i % RC4KEYLENGTH]) % 256;
			swap(RC4_SEND.RC4_STATE, i, j);
		}

		//replicate initial state to recieve state
		memcpy(RC4_RECIEVE.RC4_STATE, RC4_SEND.RC4_STATE, 256);

		this->RC4ENABLED = true;
		_printf("Initialized RC4 Encryption.");
	}

	std::string SocketHandler::CryptRC4(RCState& state, const char* in, const int& length)
	{
		int t;
		//TODO: we can have a static buffer instead of allocating for every message
		unsigned char* crypt = new unsigned char[DEFAULT_BUFLEN];

		for (int x = 0; x < length; ++x)
		{
			state.i = (state.i + 1) % 256;
			state.j = (state.j + state.RC4_STATE[state.i]) % 256;
			t = state.RC4_STATE[state.i];
			state.RC4_STATE[state.i] = state.RC4_STATE[state.j];
			state.RC4_STATE[state.j] = t;
			crypt[x] = state.RC4_STATE[(state.RC4_STATE[state.i] + state.RC4_STATE[state.j]) % 256];
		}

		std::string result;
		for (int i = 0; i < length; ++i)
		{
			result.push_back(crypt[i] ^ in[i]);
		}

		delete[] crypt;

		return result;
	}

}