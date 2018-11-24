#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>

enum Packet
{
	P_ChatMessage,
	P_Test
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool sendall(SOCKET& socket, char * data, int totalbytes);
	bool recvall(SOCKET& socket, char * data, int totalbytes);

	bool SendInt(SOCKET& socket, int _int);
	bool GetInt(SOCKET& socket, int & _int);

	bool SendPacketType(SOCKET& socket, Packet _packettype);
	bool GetPacketType(SOCKET& socket, Packet & _packettype);

	bool SendString(SOCKET& socket, std::string & _string);
	bool GetString(SOCKET& socket, std::string & _string);

	bool ProcessPacket(SOCKET& socket, Packet _packettype);

	static void ClientHandlerThread(int ID);

private:
	SOCKET Connections[100];
	int TotalConnections = 0;

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;
};

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool ConnectToServer();
	bool ConnectToPlayer(); //Connect to player (P2P)

	//Will listen for anothe rplayer connection
	bool ListenForNewConnection();

	bool SendString(std::string & _string);
	bool CloseConnection();
private: //Private functions
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();
	//Sending Funcs
	bool sendall(char * data, int totalbytes);
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);


	//Getting Funcs
	bool recvall(char * data, int totalbytes);
	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);

private:
	SOCKET Connections[100];
	int TotalConnections = 0;
	SOCKADDR_IN addr, sAddr; //Address to be binded to our Connection socket, and our server listen socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	int sizeOfSAddr = sizeof(sAddr);
	bool playerconnected;
	bool isHost;
	SOCKET serverConnection, sListen;
};

static Client * clientptr; //This client ptr is necessary so that the ClientThread method can access the Client instance/methods.
static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
