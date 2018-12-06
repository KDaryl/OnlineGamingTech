#include "Network.h"

Server::Server(const char* ip, int PORT, bool BroadcastPublically) //Port = port to broadcast on. BroadcastPublically = false if server is not open to the public (people outside of your router), true = server is open to everyone (assumes that the port is properly forwarded on router settings)
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
		addr.sin_addr.s_addr = inet_addr(ip); //Broadcast locally
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
		std::cout << "Client Connected! ID:" << TotalConnections << std::endl;
		Connections[TotalConnections] = newConnection; //Set socket in array to be the newest connection before creating the thread to handle this client's socket.
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(TotalConnections), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
		std::string MOTD = "MOTD: Welcome! This is the message of the day!.";
		SendString(Connections[TotalConnections], MOTD);
		TotalConnections += 1; //Incremenent total # of clients that have connected

		//If 2 people have joined then set one of them as the host
		if (TotalConnections == 2)
		{
			//Send message to let them know we are attempting to set a host
			std::string msg = "Migrating host";
			SendString(Connections[0], msg);
			SendString(Connections[1], msg);

			//****Randomly pick a host
			//Select connection at ID 1 as the host
			msg = "You are the host";
			SendString(Connections[1], msg);
			Sleep(100); //Wait for the host to setup itself as the server
			//Tell ID 0 to connect to 1
			msg = "Connect to";
			SendString(Connections[0], msg);
		}

		return true;
	}
}

bool Server::ProcessPacket(SOCKET& socket, Packet _packettype)
{
	switch (_packettype)
	{
	case P_ChatMessage: //Packet Type: chat message
	{
		std::string Message; //string to store our message we received
		if (!GetString(socket, Message)) //Get the chat message and store it in variable: Message
			return false; //If we do not properly get the chat message, return false
		//Next we need to send the message out to each user -> this needs to not send anything
		for (auto& connection : Connections)
		{
			if (connection == socket) //If connection is the user who sent the message...
				continue;//Skip to the next user since there is no purpose in sending the message back to the user who sent it.
			if (!SendString(connection, Message)) //Send message to connection at index i, if message fails to be sent...
			{
				std::cout << "Failed to send message from client ID: " << socket << " to client ID: " << connection << std::endl;
			}
		}
		std::cout << "Processed chat message packet from user ID: " << socket << std::endl;
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

//After we create a thread this is where the thread will run and process the connection with the passed over ID
void Server::ClientHandlerThread(int ID) //ID = the index in the SOCKET Connections array
{
	Packet PacketType;
	while (true)
	{
		if (!serverptr->GetPacketType(serverptr->Connections[ID], PacketType)) //Get packet type
			break; //If there is an issue getting the packet type, exit this loop
		if (!serverptr->ProcessPacket(serverptr->Connections[ID], PacketType)) //Process packet (packet type)
			break; //If there is an issue processing the packet, exit this loop
	}
	std::cout << "Lost connection to client ID: " << ID << std::endl;
	closesocket(serverptr->Connections[ID]);
	return;
}

bool Server::recvall(SOCKET& socket, char * data, int totalbytes)
{
	int bytesreceived = 0; //Holds the total bytes received
	while (bytesreceived < totalbytes) //While we still have more bytes to recv
	{
		int RetnCheck = recv(socket, data, totalbytes - bytesreceived, NULL); //Try to recv remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recv bytes
			return false; //Return false - failed to recvall
		bytesreceived += RetnCheck; //Add to total bytes received
	}
	return true; //Success!
}

bool Server::sendall(SOCKET& socket, char * data, int totalbytes)
{
	int bytessent = 0; //Holds the total bytes sent
	while (bytessent < totalbytes) //While we still have more bytes to send
	{
		int RetnCheck = send(socket, data + bytessent, totalbytes - bytessent, NULL); //Try to send remaining bytes
		if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
			return false; //Return false - failed to sendall
		bytessent += RetnCheck; //Add to total bytes sent
	}
	return true; //Success!
}

bool Server::SendInt(SOCKET& socket, int _int)
{
	if (!sendall(socket, (char*)&_int, sizeof(int))) //Try to send int... If int fails to send
		return false; //Return false: int not successfully sent
	return true; //Return true: int successfully sent
}

bool Server::GetInt(SOCKET& socket, int & _int)
{
	if (!recvall(socket, (char*)&_int, sizeof(int))) //Try to receive int... If int fails to be recv'd
		return false; //Return false: Int not successfully received
	return true;//Return true if we were successful in retrieving the int
}

bool Server::SendPacketType(SOCKET& socket, Packet _packettype)
{
	if (!sendall(socket, (char*)&_packettype, sizeof(Packet))) //Try to send packet type... If packet type fails to send
		return false; //Return false: packet type not successfully sent
	return true; //Return true: packet type successfully sent
}

bool Server::GetPacketType(SOCKET& socket, Packet & _packettype)
{
	if (!recvall(socket, (char*)&_packettype, sizeof(Packet))) //Try to receive packet type... If packet type fails to be recv'd
		return false; //Return false: packet type not successfully received
	return true;//Return true if we were successful in retrieving the packet type
}

bool Server::SendString(SOCKET& socket, std::string & _string)
{
	if (!SendPacketType(socket, P_ChatMessage)) //Send packet type: Chat Message, If sending packet type fails...
		return false; //Return false: Failed to send string
	int bufferlength = _string.size(); //Find string buffer length
	if (!SendInt(socket, bufferlength)) //Send length of string buffer, If sending buffer length fails...
		return false; //Return false: Failed to send string buffer length
	if (!sendall(socket, (char*)_string.c_str(), bufferlength)) //Try to send string buffer... If buffer fails to send,
		return false; //Return false: Failed to send string buffer
	return true; //Return true: string successfully sent
}

bool Server::GetString(SOCKET& socket, std::string & _string)
{
	int bufferlength; //Holds length of the message
	if (!GetInt(socket, bufferlength)) //Get length of buffer and store it in variable: bufferlength
		return false; //If get int fails, return false
	char * buffer = new char[bufferlength + 1]; //Allocate buffer
	buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
	if (!recvall(socket, buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
	{
		delete[] buffer; //delete buffer to prevent memory leak
		return false; //return false: Fails to receive string buffer
	}
	_string = buffer; //set string to received buffer message
	delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
	return true;//Return true if we were successful in retrieving the string
}
