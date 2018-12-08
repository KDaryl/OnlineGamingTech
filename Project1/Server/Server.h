#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <random>
#include <map>
#include <vector>
#include <iostream>

typedef std::pair<SOCKET, bool> sPair; //Our pair of sockets and bools
typedef std::pair<SOCKET*, SOCKET*> gamePair; //Our pair of sockets for games, first is host, second is other

enum Packet
{
	P_ChatMessage,
	P_SetupGame,
	P_Test
};

class Server
{
public:
	Server(int PORT, bool BroadcastPublically = false);
	bool ListenForNewConnection();

private:
	bool sendall(SOCKET* ID, char * data, int totalbytes);
	bool recvall(SOCKET& ID, char * data, int totalbytes);

	bool SendInt(SOCKET* ID, int _int);
	bool GetInt(SOCKET& ID, int & _int);

	bool SendPacketType(SOCKET* ID, Packet _packettype);
	bool GetPacketType(SOCKET* ID, Packet & _packettype);

	bool SendString(SOCKET* ID, std::string & _string, Packet packetType);
	bool GetString(SOCKET& ID, std::string & _string);

	bool ProcessPacket(SOCKET* ID, Packet _packettype);

	sPair findSocketPair(const SOCKET* ID);

	static void ClientHandlerThread(SOCKET* ID);

	//Generates a random integer value
	int randInt(int min, int max);

private:

	//Our map of connections, with a bool to show if they are in a game or not
	std::vector<sPair> Connections;
	std::map<SOCKET, SOCKET> Games; //The games being played

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr);
	SOCKET sListen;
};

static Server * serverptr; //Serverptr is necessary so the static ClientHandler method can access the server instance/functions.
