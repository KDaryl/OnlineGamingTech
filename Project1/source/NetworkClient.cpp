#include "Network.h"

void Client::sendData(std::string data, Packet packet)
{
	m_setDataToSend.first = data;
	m_setDataToSend.second = packet;
}

bool Client::ProcessPacket(Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage: //If packet is a chat message packet
	{
		std::string Message; //string to store our message we received
		if (!GetString(Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		std::cout << Message << std::endl; //Display the message to the user

		//Set the player as the host (the person who decides, colours, collisions etc.)
		if (Message == "You are the Host")
		{
			//Reset the start game data
			startGameData.at(0) = 99;
			startGameData.at(1) = 99;
			//Set us as the host
			isHost = true;
		}
		break;
	}

	case P_SetupGame: //If packet is a setup game packet, read in the data
	{
		std::string Message; //string to store our message we received
		if (!GetString(Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		

		startGameData.at(0) = 99;
		startGameData.at(1) = 99;
		//Get the values
		std::string valDelim = ":", variableDelim = ",";
		std::string tkn = "";
		int pos = 0, start = 0;

		//Loop twice
		for(int i = 0; i < 2; i++)
		{
			start = Message.find(valDelim);
			pos = Message.find(variableDelim);
			tkn = Message.substr(start + 1, pos - start - 1);
			std::cout << tkn << std::endl;
			Message = Message.substr(pos + variableDelim.length());
			start = pos;
			startGameData.push_back(stoi(tkn));
		}
		break;
	}

	default: //If packet type is not accounted for
		std::cout << "Unrecognized packet: " << _packettype << std::endl; //Display that packet was not found
		break;
	}
	return true;
}

void Client::ClientThread()
{
	std::vector<Packet> PacketTypes = {P_ChatMessage, P_SetupGame};
	while (true)
	{
		bool failed = true;
		for (auto& packet : PacketTypes)
		{
			if (clientptr->GetPacketType(packet)) //Get packet type
				failed = false; //Set to false if we 
			if (clientptr->ProcessPacket(packet)) //Process packet (packet type)
				failed = false;
		}

		//Break from the while loop if we failed to get/process a packet
		if (failed)
		{
			std::cout << "Failed to process or get a packet" << std::endl;
			break;
		}

	}
	std::cout << "Lost connection to the server." << std::endl;
	if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
	{
		std::cout << "Socket to the server was closed successfuly." << std::endl;
	}
	else //If connection socket was not closed properly for some reason from our function
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}

Client::Client(std::string IP, int PORT) :
	m_setDataToSend(std::pair<std::string, Packet>("EMPTY", P_ChatMessage))
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
	addr.sin_port = htons(PORT); //Port 
	addr.sin_family = AF_INET; //IPv4 Socket
	lostConnection = false;
	clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::ConnectToServer()
{
	serverConnection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(serverConnection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	std::cout << "Connected!" << std::endl;
	lostConnection = false; //Set to false as we are now connected again
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
	return true;
}

//Closes the connection to the SERVER
bool Client::CloseConnection()
{
	lostConnection = true; //Set our lost connection bool to true
	if (closesocket(serverConnection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
			return true; //return true since connection has already been closed

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

bool Client::recvall(char * data, int totalbytes)
{
	int bytesreceived = 0; //Holds the total bytes received
	while (bytesreceived < totalbytes) //While we still have more bytes to recv
	{
		int RetnCheck = recv(serverConnection, data + bytesreceived, totalbytes - bytesreceived, NULL); //Try to recv remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recv bytes
			return false; //Return false - failed to recvall
		bytesreceived += RetnCheck; //Add to total bytes received
	}
	return true; //Success!
}

bool Client::sendall(char * data, int totalbytes)
{
	int bytessent = 0; //Holds the total bytes sent
	while (bytessent < totalbytes) //While we still have more bytes to send
	{
		int RetnCheck = send(serverConnection, data + bytessent, totalbytes - bytessent, NULL); //Try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
			return false; //Return false - failed to sendall
		bytessent += RetnCheck; //Add to total bytes sent
	}
	return true; //Success!
}
bool Client::SendInt(int _int)
{
	if (!sendall((char*)&_int, sizeof(int))) //Try to send int... If int fails to send
		return false; //Return false: int not successfully sent
	return true; //Return true: int successfully sent
}

bool Client::GetInt(int & _int)
{
	if (!recvall((char*)&_int, sizeof(int))) //Try to receive int... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	return true;//Return true if we were successful in retrieving the int
}

bool Client::SendPacketType(Packet _packettype)
{
	if (!sendall((char*)&_packettype, sizeof(Packet))) //Try to send packet type... If packet type fails to send
		return false; //Return false: packet type not successfully sent
	return true; //Return true: packet type successfully sent
}

bool Client::GetPacketType(Packet & _packettype)
{
	if (!recvall((char*)&_packettype, sizeof(Packet))) //Try to receive packet type... If packet type fails to be recv'd
		return false; //Return false: packet type not successfully received
	return true;//Return true if we were successful in retrieving the packet type
}

bool Client::SendString(std::string & _string, Packet packetType)
{
	if (!SendPacketType(packetType)) //Send packet type: Chat Message, If sending packet type fails...
		return false; //Return false: Failed to send string
	int bufferlength = _string.size(); //Find string buffer length
	if (!SendInt(bufferlength)) //Send length of string buffer, If sending buffer length fails...
		return false; //Return false: Failed to send string buffer length
	if (!sendall((char*)_string.c_str(), bufferlength)) //Try to send string buffer... If buffer fails to send,
		return false; //Return false: Failed to send string buffer
	return true; //Return true: string successfully sent
}

bool Client::GetString(std::string & _string)
{
	int bufferlength; //Holds length of the message
	if (!GetInt(bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	if (!recvall(buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to receive string buffer
	}
	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	return true;//Return true if we were successful in retrieving the string
}