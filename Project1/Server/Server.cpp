#include "Server.h"

Server::Server(int PORT, bool BroadcastPublically) //Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	if (BroadcastPublically) //If server is open to public
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else //If server is only for our router
		addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
	addr.sin_port = htons(PORT); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) //Bind the address to the socket, if we fail to bind the address..
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections, if we fail to listen on listening socket...
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	serverptr = this;
}

bool Server::ListenForNewConnection()
{
	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
	if (newConnection == 0) //If accepting the client connection failed
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else //If client connection properly accepted
	{
		std::cout << "Client Connected!" << Connections.size() + 1 << std::endl;
		Connections.push_back(sPair(newConnection, false)); //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(&Connections.at(Connections.size()-1).first), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).

		//If there are enough players to fill a game (modulo two to make sure there is a pair)
		if (Connections.size() % 2 == 0)
		{
			gamePair gamePair; //Our pair
			//Loop through our connections
			//Somewhere we will have to unset the bools we set here and 
			//remove the game from our variable
			for (auto& pair : Connections)
			{
				if (nullptr == gamePair.first)
				{
					if (pair.second == false)
					{
						//Set the host
						gamePair.first = &pair.first;
						pair.second = true; //Set in game to true
					}
				}
				else if (nullptr == gamePair.second)
				{
					if (pair.second == false && (*gamePair.first != pair.first))
					{
						//Set the other player
						gamePair.second = &pair.first;
						pair.second = true; //Set in game to true
					}
				}
				else
					break; //Break out if we have our pair
			}
			
			if (nullptr != gamePair.first && nullptr != gamePair.second)
			{
				//Add the game to our vector of games being played
				Games[*gamePair.first] = *gamePair.second;

				std::string msg = "You are the Host";
				SendString(gamePair.first, msg, P_ChatMessage); //Send you are the host msg to the host selected

				std::cout << "Game created" << std::endl;
			}
		}
		return true;
	}
}

bool Server::ProcessPacket(SOCKET* ID, Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage: //Packet Type: chat message
	{
		std::string Message; //string to store our message we received
		if (!GetString(*ID, Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		//Next we need to send the message out to each user -> this needs to not send anything
		for (auto& pair : Connections)
		{
			if (pair.first == *ID) //If connection is the user who sent the message...
				continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
			if (!SendString(&pair.first, Message, P_ChatMessage)) //Send message to connection at index i, if message fails to be sent...
			{
				std::cout << "Failed to send message from a client to another" << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << ID << std::endl;
		break;
	}
	case P_SetupGame:
	{
		std::string Message; //string to store our message we received
		if (!GetString(*ID, Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		//Next we need to send the message to the other players that are part of this game
		for (auto& pair : Games)
		{
			//If we are part of this game then send the setup game packet to the other players, since it is the host who sends this message
			//We know the host is awlays the first in a game vector
			if (pair.first == *ID)
			{
				if (!SendString(&pair.second, Message, P_SetupGame)) //Send message to connection at index i, if message fails to be sent...
				{
					std::cout << "Failed to send setup packet from a client to another" << std::endl;
				}
				break; //Can stop looping as we have found the other players
			}
		}
		std::cout << "Processed SetupGame packet from user ID: " << ID << std::endl;
		break;
	}

	default: //If packet type is not accounted for
	{
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	}
	return true;
}

sPair Server::findSocketPair(const SOCKET * ID)
{
	//The pair we want to find
	sPair foundConnection;
	for (auto& pair : serverptr->Connections)
	{
		//If the socket in that pair is equal to our socket that sbeing handled here in this thread
		if (pair.first == *ID)
		{
			//Set our pair as found
			foundConnection = pair;
			break;
		}
	}

	//Return the found pair
	return foundConnection;
}

//After we create a thread this is where the thread will run and process the connection with the passed over ID
void Server::ClientHandlerThread(SOCKET* ID) //ID = the index in the SOCKET Connections array
{
	const SOCKET* host = nullptr;
	SOCKET* other;
	//If player was in a game, remove them from the map
	for (auto& pair : serverptr->Games)
	{
		if (pair.first == *ID)
		{
			host = &pair.first;
			other = &pair.second;
			auto otherPlayer = serverptr->findSocketPair(&pair.second);
			otherPlayer.second = false;
			break;
		}
		else if (pair.second == *ID)
		{
			host = &pair.first;
			other = &pair.second;
			auto otherPlayer = serverptr->findSocketPair(&pair.first);
			otherPlayer.second = false;
			break;
		}
	}

	Packet packettype;
	while (true)
	{
		if (!serverptr->GetPacketType(ID, packettype)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(ID, packettype)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;

	//The pair we want to find
	auto foundConnection = serverptr->findSocketPair(ID);
	//get the iterator that points to our socket
	auto pos = std::find(serverptr->Connections.begin(), serverptr->Connections.end(), foundConnection);
	//Get the index of that iterator
	auto index = std::distance(serverptr->Connections.begin(), pos) - 1;


	//if this player was part of a game, delete the game for the vector
	if(nullptr != host)
		serverptr->Games.erase(*host);

	//Close the socket
	closesocket(serverptr->Connections.at(index).first);
	//Remove socket from our vector of connections
	if(pos != serverptr->Connections.end())
		serverptr->Connections.erase(pos);
	return;
}

int Server::randInt(int min, int max)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(min, max); //Ste min and max values for random number
	//Return a random value between min and max
	return dis(gen);
}
