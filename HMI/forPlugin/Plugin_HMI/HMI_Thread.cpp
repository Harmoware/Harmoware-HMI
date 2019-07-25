#pragma execution_character_set("utf-8")

#include "HMI_Thread.h"
#include"CommToAuto.h"
#include "mmsystem.h"
#include<fstream>
#include<iostream>
#include<string>
#include<map>

int traficjam = 0;

static void mainThread(void *param)
{
	HMI_Thread *hmi = (HMI_Thread *)param;
	hmi->run();
}

void HMI_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/*create Multi thread*/
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);

}


void HMI_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;

	turn_right_enable = false;
	turn_left_enable = false;
	go_straight_enable = false;

	string ip, in_port, out_port;

	//loadConfig("config.txt", &ip, &in_port, &out_port);
	//m_Comm.InitClient(ip, in_port, out_port);
//	m_Comm.InitClient("192.168.100.29", "10004", "10003");   //�������Ŏ����p
	m_Comm.InitClient("192.168.3.10", "10004", "10003");   //�{�ԗp
//	m_Comm.InitClient("192.168.1.238", "10004", "10003");   //�{�ԗp
}

HMI_Thread::HMI_Thread()
{
	initialize();
}

void HMI_Thread::vehicle_event(MMDAgent *mmdagent, const char *str) {
	HMI_MSG msg;
	msg.type = COMMAND_MSG;
	msg = HMI_MSG::FromString(m_Comm.m_LatestClientMsg);

	//***********************************//
	//----- �o�́F����w�� ------
	msg.options.clear();
	//mmdagent->sendMessage("HMI debug message", "%s", "vehicle event!");

	if (strcmp(str, "�E") == 0 && turn_right_enable == true) { //run()�Ȃ��邱�Ƃ��ł��邩�`�F�b�N�ŉE�ɋȂ����ꍇ
		msg.options.push_back(RIGHT_TURN_ACTION);	// RIGHT_TURN_ACTION
	}
	else if (strcmp(str, "�E") == 0 && turn_right_enable == false) {//run()�Ȃ��邱�Ƃ��ł��邩�`�F�b�N�ŉE�ɋȂ���Ȃ��ꍇ
		mmdagent->sendMessage("�Ȃ���", "%s", "NG");
	}
	else if (strcmp(str, "��") == 0) {
		msg.options.push_back(LEFT_TURN_ACTION);	// FORWARD_ACTION, LEFT_TURN_ACTION
	}
	else if (strcmp(str, "���i") == 0) {
		msg.options.push_back(FORWARD_ACTION);	// FORWARD_ACTION, FORWARD_ACTION
	}
	else if (strcmp(str, "�a��") == 0) {
		traficjam = 1;
		//mmdagent->sendMessage("trafic_situation", "%s", "traffic_jam");
	}
	else if (strcmp(str, "����") == 0) {
		msg.options.push_back(SLOWDOWN_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("����", "%s", "OK");
	}
	else if (strcmp(str, "�X�s�[�h����") == 0) {
		msg.options.push_back(SLOWDOWN_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("�X�s�[�h����", "%s", "OK");
	}
	else if (strcmp(str, "���x�����ǂ�") == 0) {
		msg.options.push_back(START_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("���x", "%s", "���ǂ�");
	}
	else if (strcmp(str, "�X�s�[�h�����ǂ�") == 0) {
		msg.options.push_back(START_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("�X�s�[�h", "%s", "���ǂ�");
	}
	else if (strcmp(str, "�~�߂�") == 0) {
		msg.options.push_back(STOP_ACTION);	// STOP_ACTION
		mmdagent->sendMessage("�~�߂�", "%s", "OK");
	}
	else if (strcmp(str, "���i") == 0) {
		msg.options.push_back(START_ACTION);	// START_ACTION
		mmdagent->sendMessage("set_start", "%s", "���i");
	}
	else if (strcmp(str, "�o��") == 0) {
		msg.options.push_back(START_ACTION);	// START_ACTION
		mmdagent->sendMessage("set_start", "%s", "�o��");
	}
	else if (strcmp(str, "IB��") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 0;
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
	}
	else if (strcmp(str, "���É��i�V���i���Z���^�[") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
	}
	else if (strcmp(str, "�w�Z") == 0) {
		msg.next_destination_id = msg.data2[msg.data1[str]];
		//msg.next_destination_id = 4;
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
		//mmdagent->sendMessage("set_route", "%d", data2[msg.data1[str]]);
		//mmdagent->sendMessage("set_route", "%d", msg.data2["The School"]);
	}
	else if (strcmp(str, "�a�@") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 1; //�{��1�f���p
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
	}
	m_Comm.sendActionPackets(msg);
}

void HMI_Thread::vehicle_stop_event(MMDAgent *mmdagent, const char *str) {
	HMI_MSG msg;
	msg.type = COMMAND_MSG;
	msg = HMI_MSG::FromString(m_Comm.m_LatestClientMsg);

	//***********************************//
	//----- �o�́F����w�� ------
	msg.options.clear();
	//mmdagent->sendMessage("HMI debug message", "%s", "vehicle event!");

	if (strcmp(str, "�H�w��") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("eye_stop", "%s", str);
	}
	else if (strcmp(str, "IB��") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 0; //�{��1�f���p
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("eye_stop", "%s", str);
	}
	else if (strcmp(str, "�a�@") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 1; //�{��1�f���p
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("eye_stop", "%s", str);
	}
	m_Comm.sendActionPackets(msg);
}

void HMI_Thread::run()
{
	MMDAgent *mmdagent;
	mmdagent = m_mmdagent;

	HMI_MSG msg;
	msg.type = COMMAND_MSG;
	msg.bErr = 0;


	int data_length = 0;

	bool bEnableDirection[DIRECTION_NUM];
	bool pre_bEnableDirection[DIRECTION_NUM];

	bool flag_EnableDirectionChange = false;

	ACTION_TYPE PreviousSelection = STOP_ACTION;
	int PreviousCurrID = -1;

	// initialize bEnableDirection
	for (int i = 0; i < DIRECTION_NUM; i++) {
		pre_bEnableDirection[i] = false;
	}

	int cnt = 0;
	while (1) {
		Sleep(100);

		if (m_Comm.ReConnect() < 0)
		{
			continue;
		}

		Sleep(100);

		//----- �f�[�^��M ------
		char network_data[MAX_PACKET_SIZE];
		data_length = m_Comm.receiveClientData(network_data);
		if (data_length <= 0) { //no data recieved
			mmdagent->sendMessage("HMI debug message", "%s", "No data!");
			continue;
		}
		/*�f���̎��́A�R�����g�A�E�g�����ق�����������*/
		//mmdagent->sendMessage("### receive", "%d", msg.current);
		//->sendMessage("VEHICLE_RECEIVE", "%s", LPCTSTR(m_Comm.m_LatestClientMsg.c_str()));

		//----- msg�^�ɕϊ� ------
		m_Comm.m_LatestClientMsg = string(network_data, data_length);
		msg = HMI_MSG::FromString(m_Comm.m_LatestClientMsg);

		//----- ����������`�F�b�N�i�I�v�V�����ǂݍ��݁j ------
		flag_EnableDirectionChange = false;
		if (msg.type == OPTION_MSG)
		{
			// initialize bEnableDirection
			for (int i = 0; i < DIRECTION_NUM; i++) {
				bEnableDirection[i] = false;
			}

			// check options
			for (unsigned int i = 0; i < msg.options.size(); i++)
			{
				if (msg.options.at(i) == FORWARD_ACTION)
					bEnableDirection[0] = true;
				else if (msg.options.at(i) == LEFT_TURN_ACTION)
					bEnableDirection[1] = true;
				else if (msg.options.at(i) == RIGHT_TURN_ACTION)
					bEnableDirection[2] = true;
			}

			//�Ȃ��邱�Ƃ��ł��邩�`�F�b�N
			if (bEnableDirection[0] == true) {
				go_straight_enable = true;
			}
			else {
				go_straight_enable = false;
			}

			if (bEnableDirection[1] == true) {
				turn_left_enable = true;
			}
			else {
				turn_left_enable = false;
			}

			if (bEnableDirection[2] == true) {
				turn_right_enable = true;
			}
			else {
				turn_right_enable = false;
			}

			//----- ������������ς�����Ƃ��̏��� ------
			//�O�̒l�Ƃ̔�r
			for (int i = 0; i < DIRECTION_NUM; i++) {
				if (pre_bEnableDirection[i] != bEnableDirection[i]) {
					flag_EnableDirectionChange = true;
					break;
				}
			}

			//------�E�ɋȂ����ꍇ�i�V�X�e������b��������j-----
			/*if (bEnableDirection[2] == true) {
				if (pre_bEnableDirection[2] != bEnableDirection[2]) {
					mmdagent->sendMessage("ENABLE_VEHICLE_ACTION", "%s", "RIGHT_TURN");
				}
			}*/

			/*if (traficjam == 1) {
				if (msg.current == FORWARD_ACTION) {
					if (bEnableDirection[1] == true) {
						mmdagent->sendMessage("�a��", "%s", "LEFT_OK");
					}
					else if (bEnableDirection[2] == true) {
						mmdagent->sendMessage("�a��", "%s", "RIGHT_OK");
					}
					else {
						mmdagent->sendMessage("�a��", "%s", "NO");
					}
				}
				else if (msg.current == LEFT_TURN_ACTION) {
					if (bEnableDirection[0] == true) {
						mmdagent->sendMessage("�a��", "%s", "FORWARD_OK");
					}
					else if (bEnableDirection[2] == true) {
						mmdagent->sendMessage("�a��", "%s", "RIGHT_OK");
					}
					else {
						mmdagent->sendMessage("�a��", "%s", "NO");
					}
				}
				else if (msg.current == RIGHT_TURN_ACTION) {
					if (bEnableDirection[0] == true) {
						mmdagent->sendMessage("�a��", "%s", "FORWARD_OK");
					}
					else if (bEnableDirection[1] == true) {
						mmdagent->sendMessage("�a��", "%s", "LEFT_OK");
					}
					else {
						mmdagent->sendMessage("�a��", "%s", "NO");
					}
				}
				traficjam = 0;
			}*/


			//���̒l���A�O�̒l�Ƃ��ĕۑ�
			for (int i = 0; i < DIRECTION_NUM; i++) {
				pre_bEnableDirection[i] = bEnableDirection[i];
			}

			//�ς���Ă���ꍇ�̏���
			if (flag_EnableDirectionChange) {
				// ����FST�Ŏ󂯂�`�ŏo�͂�����
				m_Comm.m_LatestClientMsg = "Message>" + std::to_string(cnt) + " " + m_Comm.m_LatestClientMsg;
				mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", LPCTSTR(m_Comm.m_LatestClientMsg.c_str()));
			}

			/*
			if(�a�؏�񂪗�����){
			mmdagent->sendMessage("traffic_situation", "%s", "traffic_jam");
			//���݁A�a�؂Ői�H�ύX�ł��邩�̊m�F�̂��߂ɓK���ɐi�H��ύX���Ă���̂Ō�ŕύX����
			}
			*/


			//----- ���������`�F�b�N�i�I�v�V�����ǂݍ��݁j ------
			//----- ������������ς�����Ƃ��̏��� ------
			//�O�̒l�Ƃ̔�r
			/*if (PreviousSelection != msg.current || PreviousCurrID != msg.currID) {
			//�ς���Ă���ꍇ�̏���
			if (msg.current == FORWARD_ACTION) {
			mmdagent->sendMessage("VEHICLE_RECEIVE", "%s","FORWARD_ACTION");
			}
			else if (msg.current == RIGHT_TURN_ACTION) {
			mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "RIGHT_TURN_ACTION");
			}
			else if (msg.current == LEFT_TURN_ACTION) {
			mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "LEFT_TURN_ACTION");
			}
			}*/

			//----- ���������`�F�b�N�i�I�v�V�����ǂݍ��݁j ------
			//----- ������������ς�����Ƃ��̏��� ------
			//�O�̒l�Ƃ̔�r
			if (PreviousSelection != msg.current) {
				//�ς���Ă���ꍇ�̏���
				if (msg.current == FORWARD_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "FORWARD_ACTION");
				}
				else if (msg.current == RIGHT_TURN_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "RIGHT_TURN_ACTION");
				}
				else if (msg.current == LEFT_TURN_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "LEFT_TURN_ACTION");
				}
				else if (msg.current == START_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "START_ACTION");
				}
				else if (msg.current == STOP_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "STOP_ACTION");
				}
				else if (msg.current == SLOWDOWN_ACTION) {
					mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", "SLOWDOWN_ACTION");
				}
			}

			//���̒l���A�O�̒l�Ƃ��ĕۑ�
			PreviousSelection = msg.current;
			PreviousCurrID = msg.currID;

		}
	}
}