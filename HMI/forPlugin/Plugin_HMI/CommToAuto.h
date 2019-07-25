#pragma once
#pragma execution_character_set("utf-8")


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<string>
#include <vector>
#include <sstream>
#include<map>

#define DEFAULT_PORT "10080"
#define DEFAULT_PROTO SOCK_STREAM
#define DEFAULT_BUFLEN 512
#define MAX_PACKET_SIZE 1000000

#define DIRECTION_NUM 3

using namespace std;



enum PacketTypes {

	INIT_CONNECTION = 0,

	ACTION_EVENT = 1,

};

struct Packet {

	unsigned int packet_type;

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};

enum MSG_TYPE { COMMAND_MSG = 0, CONFIRM_MSG = 1, OPTION_MSG = 2, CURR_OPTION_MSG = 3, UNKNOWN_MSG = 5 };
enum ACTION_TYPE {
	FORWARD_ACTION = 0, BACKWARD_ACTION = 1, STOP_ACTION = 2,
	LEFT_TURN_ACTION = 3, RIGHT_TURN_ACTION = 4, U_TURN_ACTION = 5, SWERVE_ACTION = 6,
	OVERTACK_ACTION = 7, START_ACTION = 8, SLOWDOWN_ACTION = 9, CHANGE_DESTINATION = 10,WAITING_ACTION = 11,DESTINATION_REACHED=12
};
class HMI_MSG
{
public:
	MSG_TYPE type;
	std::vector<ACTION_TYPE> options;
	ACTION_TYPE current;
	int currID;
	bool bErr;
	std::string err_msg;
	int next_destination_id;
	std::vector<std::string> destinations;
	std::map<std::string, std::string> data1;
	std::map<std::string, int> data2;

	HMI_MSG()
	{
		next_destination_id = -1;
		currID = -1;
		type = OPTION_MSG;
		bErr = false;

		data1["病院"] = "The Hospital";
		data1["学校"] = "The School";
		data1["家"] = "Home";
		data1["仕事場"] = "Work";
		data1["ショッピングモール"] = "The Mall";
		data1["IB館"] = "IB";
	}

	static HMI_MSG FromString(string msg)
	{
		HMI_MSG recieved_msg;
		vector<string> sections = SplitString(msg, ",");
		if (sections.size() == 8)
		{
			int type_str = atoi(sections.at(0).c_str());
			switch (type_str)
			{
			case 0:
				recieved_msg.type = COMMAND_MSG;
				break;
			case 1:
				recieved_msg.type = CONFIRM_MSG;
				break;
			case 2:
				recieved_msg.type = OPTION_MSG;
				break;
			case 3:
				recieved_msg.type = CURR_OPTION_MSG;
				break;
			default:
				recieved_msg.type = UNKNOWN_MSG;
				break;
			}

			vector<string> directions = SplitString(sections.at(1), ";");
			for (unsigned int i = 0; i < directions.size(); i++)
			{
				int idirect = atoi(directions.at(i).c_str());
				if (idirect == 0)
					recieved_msg.options.push_back(FORWARD_ACTION);
				else if (idirect == 3)
					recieved_msg.options.push_back(LEFT_TURN_ACTION);
				else if (idirect == 4)
					recieved_msg.options.push_back(RIGHT_TURN_ACTION);
				else if (idirect == 2)
					recieved_msg.options.push_back(STOP_ACTION);
				else if (idirect == 8)
					recieved_msg.options.push_back(START_ACTION);
				else if (idirect == 9)
					recieved_msg.options.push_back(SLOWDOWN_ACTION);
				else if (idirect == 10)
					recieved_msg.options.push_back(CHANGE_DESTINATION);
				else if (idirect == 11)
					recieved_msg.options.push_back(WAITING_ACTION);
				else if (idirect == 12)
					recieved_msg.options.push_back(DESTINATION_REACHED);

			}
			int idir_curr = atoi(sections.at(2).c_str());
			if (idir_curr == 0)
				recieved_msg.current = FORWARD_ACTION;
			else if (idir_curr == 3)
				recieved_msg.current = LEFT_TURN_ACTION;
			else if (idir_curr == 4)
				recieved_msg.current = RIGHT_TURN_ACTION;

			recieved_msg.currID = atoi(sections.at(3).c_str());
			recieved_msg.bErr = atoi(sections.at(4).c_str());
			recieved_msg.err_msg = sections.at(5);
			recieved_msg.next_destination_id = atoi(sections.at(6).c_str());
			recieved_msg.destinations = SplitString(sections.at(7), ";");

			for (unsigned int i = 0; i < recieved_msg.destinations.size(); i++) {
				recieved_msg.data2[recieved_msg.destinations[i]] = i;
			}

		}
		return recieved_msg;
	}

	static vector<string> SplitString(const string& str, const string& token)
	{
		vector<string> str_parts;
		int iFirstPart = 0;
		int iSecondPart = str.find(token, iFirstPart);

		while (iSecondPart > 0 && iSecondPart < str.size())
		{
			str_parts.push_back(str.substr(iFirstPart, iSecondPart - iFirstPart));
			iFirstPart = iSecondPart + 1;
			iSecondPart = str.find(token, iFirstPart);
		}

		return str_parts;
	}

	std::string CreateStringMessage()
	{
		std::ostringstream oss;
		oss << type << ",";
		for (unsigned int i = 0; i< options.size(); i++)
			oss << options.at(i) << ";";

		oss << "," << current;
		oss << "," << currID;
		oss << "," << bErr;
		oss << "," << err_msg;
		oss << "," << next_destination_id << ",";

		for (unsigned int i = 0; i< destinations.size(); i++)
			oss << destinations.at(i) << ";";

		oss << ",";

		return oss.str();
	}


};

class NetworkServices
{
public:
	static int sendMessage(SOCKET curSocket, char * message, int messageSize)
	{
		return send(curSocket, message, messageSize, 0);
	}
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
	{
		return recv(curSocket, buffer, bufSize, 0);
	}
};

class CommToAuto
{
public:
	HMI_MSG m_CurrMessage;
	CommToAuto();
	~CommToAuto();
	int InitClient(string server_name, string port_send, string port_receive);
	void sendActionPackets(HMI_MSG msg);
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	SOCKET ConnectSocket;
	string m_serverName;
	string m_PortSend;
	string m_PortReceive;
	int ReConnect();
	int ReConnectClient();
	int receiveClientData(char *);
	void DoOneClientStep();
	static void clientLoop(void * arg);

	string m_LatestClientMsg;
	vector<string> m_destinations;
};

