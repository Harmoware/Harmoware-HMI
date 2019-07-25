
#include "stdafx.h"
#include "CommToAuto.h"
#include <iostream>
// used for multi-threading
#include <process.h>
#include <sstream>



CommToAuto::CommToAuto()
{

}


CommToAuto::~CommToAuto()
{
}

int CommToAuto::ReConnect()
{
	// create WSADATA object
	WSADATA wsaData;

	// socket
	ConnectSocket = INVALID_SOCKET;

	// holds address info for socket to connect to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return -1;
	}

	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

									  //resolve server address and port 
	iResult = getaddrinfo(m_serverName.c_str(), m_PortReceive.c_str(), &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return  -1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return  -1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}



	// no longer need address info for server
	freeaddrinfo(result);



	// if connection failed
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return  -1;
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return  -1;
	}

	//disable nagle
	char value = 1;
	setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}
int CommToAuto::ReConnectClient()
{
	// create WSADATA object
	WSADATA wsaData;

	// socket
	ClientSocket = INVALID_SOCKET;

	// holds address info for socket to connect to
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return -1;
	}



	// set address info
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

									  //resolve server address and port 
	iResult = getaddrinfo(m_serverName.c_str(), m_PortSend.c_str(), &hints, &result);

	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return  -1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ClientSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return  -1;
		}

		// Connect to server.
		iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			printf("The server is down... did not connect");
		}
	}



	// no longer need address info for server
	freeaddrinfo(result);



	// if connection failed
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return  -1;
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;

	iResult = ioctlsocket(ClientSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return  -1;
	}

	//disable nagle
	char value = 1;
	setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}

int CommToAuto::InitClient(string server_name, string port_send, string port_receive)
{
	m_serverName = server_name;
	m_PortSend = port_send;
	m_PortReceive = port_receive;

	//	_beginthread(CommToAuto::clientLoop, 0, this);

	return 1;
}

void CommToAuto::sendActionPackets(HMI_MSG msg)
{
	if (ReConnectClient() >= 0)
	{
		/*ostringstream str_msg;
		str_msg << msg.type << ",";

		for (unsigned int i = 0; i < msg.options.size(); i++)
		str_msg << msg.options.at(i) << ";";
		str_msg << "," << msg.current;
		str_msg << "," << msg.currID;
		str_msg << "," << msg.bErr;
		str_msg << "," << msg.err_msg << ",";*/

		//-----------------------------------------------------------
		/*for (unsigned int i = 0; i < msg.options.size(); i++)
			str_msg << msg.options.at(i) << ";";
		str_msg << ",,," << msg.bErr << ",";
		str_msg << msg.err_msg << ",";*/
		//-------------------------------------------------------------

		/*string final_str(str_msg.str());

		NetworkServices::sendMessage(ClientSocket, (char*)final_str.c_str(), final_str.size());*/

		string final_str = msg.CreateStringMessage();

		NetworkServices::sendMessage(ClientSocket, (char*)final_str.c_str(), final_str.size());
		

		shutdown(ClientSocket, 0);
		closesocket(ClientSocket);
		WSACleanup();
	}
}

int CommToAuto::receiveClientData(char * recvbuf)
{
	int iResult = NetworkServices::receiveMessage(ConnectSocket, recvbuf, MAX_PACKET_SIZE);

	if (iResult == 0)
	{
		printf("Connection closed\n");
		shutdown(ConnectSocket, 0);
		closesocket(ConnectSocket);
		WSACleanup();
		return -1;
	}

	return iResult;
}

void CommToAuto::DoOneClientStep()
{
	if (ReConnect() < 0)
	{

		return;
	}



	Sleep(100);

	Packet packet;
	char network_data[MAX_PACKET_SIZE];
	int data_length = receiveClientData(network_data);

	if (data_length <= 0)
	{

		return;
	}

	m_LatestClientMsg = string(network_data, data_length);


	HMI_MSG msg = HMI_MSG::FromString(m_LatestClientMsg);

	m_CurrMessage = msg;


	if (msg.type == OPTION_MSG)
	{
		bool bEnableForward = false;
		bool bEnableLeft = false;
		bool bEnableRight = false;

		for (unsigned int i = 0; i < msg.options.size(); i++)
		{

			if (msg.options.at(i) == FORWARD_ACTION)
				bEnableForward = true;
			else if (msg.options.at(i) == LEFT_TURN_ACTION)
				bEnableLeft = true;
			else if (msg.options.at(i) == RIGHT_TURN_ACTION)
				bEnableRight = true;
		}

		//}


		for (unsigned int i = 0; i < msg.destinations.size(); i++)
		{
			//check if already exit
			bool bFound = false;
			for (unsigned int j = 0; j < m_destinations.size(); j++)
			{
				if (m_destinations.at(j).compare(msg.destinations.at(i)) == 0)
					bFound = true;
			}
			if (bFound == false)
			{
				m_destinations.push_back(msg.destinations.at(i));
				//msg.data2[msg.destinations.at(i)] = i;

				LVITEM lvItem;

				lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				//lvItem.pszText = LPSTR(msg.destinations.at(i).c_str());
				lvItem.iImage = i;
				//m_pDestinationList->InsertItem(&lvItem);
			}
		}

		if (msg.next_destination_id != -1)
		{
			//m_pDestinationList->SetHotItem(msg.next_destination_id);
		}
	}

	//m_pInfoBox->GetCount();
	//AfxMessageBox(LPCTSTR(m_LatestClientMsg.c_str()));
	//cout << m_LatestClientMsg << endl;

}

void CommToAuto::clientLoop(void * arg)
{
	CommToAuto* pCom = (CommToAuto*)arg;

	while (true)
	{
		pCom->DoOneClientStep();

	}
}


