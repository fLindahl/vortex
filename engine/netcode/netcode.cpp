#include "config.h"
#include "netcode.h"
#include <windows.h>
#include <ws2tcpip.h>
#include <string>

namespace Netcode
{
//-------------------------------------------------------------------------
/**
*/
SocketHandler::SocketHandler()
{
	RC4KEY = "banan";
	ConnectSocket = INVALID_SOCKET;
	bufLength = 0;
}

//-------------------------------------------------------------------------
/**
*/
SocketHandler::~SocketHandler()
{
	
}

//-------------------------------------------------------------------------
/**
*/
bool SocketHandler::initSocket(const char* ip)
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		_error("WSAStartup failed: %d", iResult);
		return false;
	}
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//std::string sargv = "127.0.0.1";
	//const char* argv = sargv.c_str();

	iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		_error("getaddrinfo failed: %d", iResult);
		WSACleanup();
		return false;
	}

	ptr = result;

	// Create a SOCKET for connecting to server
	this->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (this->ConnectSocket == INVALID_SOCKET)
	{
		_error("Error at socket(): %ld", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Connect to server.
	iResult = connect(this->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		_error("Unable to connect to server! %ld", WSAGetLastError());
		closesocket(this->ConnectSocket);
		this->ConnectSocket = INVALID_SOCKET;			
	}

	freeaddrinfo(result);

	if (this->ConnectSocket == INVALID_SOCKET) {
		WSACleanup();
		return false;
	}

	//Non Blocking
	u_long iMode = 1;
	ioctlsocket(ConnectSocket, FIONBIO, &iMode);

	JoinMsg join;
	char sendbuf[sizeof(join)];

	//filling join message
	join.head.id = 0;
	join.head.length = sizeof(join);
	join.head.type = Join;

	//copying join message to sendbuf
	memcpy((void*)sendbuf, (void*)&join, sizeof(join));

	// Send an initial buffer
	this->sendData(sendbuf, sizeof(sendbuf));

	if (iResult == SOCKET_ERROR) {
		_error("Socket shutdown failed: %d", WSAGetLastError());
		closesocket(this->ConnectSocket);
		WSACleanup();
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------
/**
*/
void SocketHandler::close()
{
	//Shutdown the socket!
	iResult = shutdown(this->ConnectSocket, SD_SEND);
	closesocket(this->ConnectSocket);
	WSACleanup();
}

//-------------------------------------------------------------------------
/**
*/
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

	// Send buffer
	iResult = send(this->ConnectSocket, buf, size, 0);

	if (iResult == SOCKET_ERROR) 
	{
		_warning("Packet Send failed: %d", WSAGetLastError());
		closesocket(this->ConnectSocket);
		WSACleanup();
		return false;
	}
	_printf("Bytes Sent: %ld\n", iResult);
	return true;
}

//-------------------------------------------------------------------------
/**
*/
char* SocketHandler::recieveData()
{
	int recvbuflen = DEFAULT_BUFLEN;

	int iResult = recv(this->ConnectSocket, this->recvbuf, recvbuflen, 0);
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
	else if (iResult == 0)
	{
		_printf("Connection closed.");
		return NULL;
	}
	else
	{
		//printf("recv failed: %d\n", WSAGetLastError());
		return NULL;
	}
}

//-------------------------------------------------------------------------
/**
*/
SOCKET SocketHandler::getSocket()
{
	return this->ConnectSocket;
}

//-------------------------------------------------------------------------
/**
*/
int SocketHandler::getBufLength()
{
	return this->bufLength;
}


//-------------------------------------------------------------------------
/**
	Swap two values of an array.

	Uses some bitlevel hackery to save performance and memory.
*/
void SocketHandler::swap(unsigned char *s, unsigned int i, unsigned int j)
{
	s[i] ^= s[j];
	s[j] ^= s[i];
	s[i] ^= s[j];
}

//-------------------------------------------------------------------------
/**
	Initialize RC4 encrypting/decrypting
*/
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

//------------------------------------------------------------------------
/**
	Encrypt/Decrypt a packet

	@todo	Make the crypt allocation once
	@todo	Replace std::string with char buffer
*/
std::string SocketHandler::CryptRC4(RCState& state, const char* in, const int& length)
{
	int t;

	//This should be done once!
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


