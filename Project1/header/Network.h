#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

enum Packet
{
	P_ChatMessage,
	P_SetupGame,
	P_Test
};

class Client
{
public: //Public functions
	Client(std::string IP, int PORT);
	bool ConnectToServer();

	bool SendString(std::string & _string, Packet packetType);
	bool CloseConnection();

	//Bool to get wheter we are the host or not
	bool selectedAsHost() { return isHost; }
	bool startGame() {return startTheGame; }
	bool lostConnecion() { return lostConnection; }

	void sendData(std::string data, Packet packet);

	std::map<std::string, int> m_setupGameData;

	std::vector<int> startGameData;

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

	SOCKET Connections[100];
	int TotalConnections = 0;
	SOCKADDR_IN addr, sAddr; //Address to be binded to our Connection socket, and our server listen socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	int sizeOfSAddr = sizeof(sAddr);
	bool startTheGame;
	bool isHost;
	bool lostConnection;
	SOCKET serverConnection;

	std::pair<std::string, Packet> m_setDataToSend; //We will set this outside of our class and send it during our thread handler
};

static Client * clientptr; //This client ptr is necessary so that the ClientThread method can access the Client instance/methods.
