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
//	m_Comm.InitClient("192.168.100.29", "10004", "10003");   //研究室で試す用
	m_Comm.InitClient("192.168.3.10", "10004", "10003");   //本番用
//	m_Comm.InitClient("192.168.1.238", "10004", "10003");   //本番用
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
	//----- 出力：動作指示 ------
	msg.options.clear();
	//mmdagent->sendMessage("HMI debug message", "%s", "vehicle event!");

	if (strcmp(str, "右") == 0 && turn_right_enable == true) { //run()曲がることができるかチェックで右に曲がれる場合
		msg.options.push_back(RIGHT_TURN_ACTION);	// RIGHT_TURN_ACTION
	}
	else if (strcmp(str, "右") == 0 && turn_right_enable == false) {//run()曲がることができるかチェックで右に曲がれない場合
		mmdagent->sendMessage("曲がる", "%s", "NG");
	}
	else if (strcmp(str, "左") == 0) {
		msg.options.push_back(LEFT_TURN_ACTION);	// FORWARD_ACTION, LEFT_TURN_ACTION
	}
	else if (strcmp(str, "直進") == 0) {
		msg.options.push_back(FORWARD_ACTION);	// FORWARD_ACTION, FORWARD_ACTION
	}
	else if (strcmp(str, "渋滞") == 0) {
		traficjam = 1;
		//mmdagent->sendMessage("trafic_situation", "%s", "traffic_jam");
	}
	else if (strcmp(str, "減速") == 0) {
		msg.options.push_back(SLOWDOWN_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("減速", "%s", "OK");
	}
	else if (strcmp(str, "スピード減速") == 0) {
		msg.options.push_back(SLOWDOWN_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("スピード減速", "%s", "OK");
	}
	else if (strcmp(str, "速度をもどす") == 0) {
		msg.options.push_back(START_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("速度", "%s", "もどす");
	}
	else if (strcmp(str, "スピードをもどす") == 0) {
		msg.options.push_back(START_ACTION);	// SLOWDOWN_ACTION
		mmdagent->sendMessage("スピード", "%s", "もどす");
	}
	else if (strcmp(str, "止める") == 0) {
		msg.options.push_back(STOP_ACTION);	// STOP_ACTION
		mmdagent->sendMessage("止める", "%s", "OK");
	}
	else if (strcmp(str, "発進") == 0) {
		msg.options.push_back(START_ACTION);	// START_ACTION
		mmdagent->sendMessage("set_start", "%s", "発進");
	}
	else if (strcmp(str, "出発") == 0) {
		msg.options.push_back(START_ACTION);	// START_ACTION
		mmdagent->sendMessage("set_start", "%s", "出発");
	}
	else if (strcmp(str, "IB館") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 0;
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
	}
	else if (strcmp(str, "名古屋ナショナルセンター") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
	}
	else if (strcmp(str, "学校") == 0) {
		msg.next_destination_id = msg.data2[msg.data1[str]];
		//msg.next_destination_id = 4;
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("set_route", "%s", str);
		//mmdagent->sendMessage("set_route", "%d", data2[msg.data1[str]]);
		//mmdagent->sendMessage("set_route", "%d", msg.data2["The School"]);
	}
	else if (strcmp(str, "病院") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 1; //本来1デモ用
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
	//----- 出力：動作指示 ------
	msg.options.clear();
	//mmdagent->sendMessage("HMI debug message", "%s", "vehicle event!");

	if (strcmp(str, "工学部") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("eye_stop", "%s", str);
	}
	else if (strcmp(str, "IB館") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 0; //本来1デモ用
		msg.options.push_back(CHANGE_DESTINATION);
		mmdagent->sendMessage("eye_stop", "%s", str);
	}
	else if (strcmp(str, "病院") == 0) {
		//msg.next_destination_id = msg.data2[msg.data1[str]];
		msg.next_destination_id = 1; //本来1デモ用
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

		//----- データ受信 ------
		char network_data[MAX_PACKET_SIZE];
		data_length = m_Comm.receiveClientData(network_data);
		if (data_length <= 0) { //no data recieved
			mmdagent->sendMessage("HMI debug message", "%s", "No data!");
			continue;
		}
		/*デモの時は、コメントアウトしたほうがいいかも*/
		//mmdagent->sendMessage("### receive", "%d", msg.current);
		//->sendMessage("VEHICLE_RECEIVE", "%s", LPCTSTR(m_Comm.m_LatestClientMsg.c_str()));

		//----- msg型に変換 ------
		m_Comm.m_LatestClientMsg = string(network_data, data_length);
		msg = HMI_MSG::FromString(m_Comm.m_LatestClientMsg);

		//----- 動ける方向チェック（オプション読み込み） ------
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

			//曲がることができるかチェック
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

			//----- 動ける方向が変わったときの処理 ------
			//前の値との比較
			for (int i = 0; i < DIRECTION_NUM; i++) {
				if (pre_bEnableDirection[i] != bEnableDirection[i]) {
					flag_EnableDirectionChange = true;
					break;
				}
			}

			//------右に曲がれる場合（システムから話しかける）-----
			/*if (bEnableDirection[2] == true) {
				if (pre_bEnableDirection[2] != bEnableDirection[2]) {
					mmdagent->sendMessage("ENABLE_VEHICLE_ACTION", "%s", "RIGHT_TURN");
				}
			}*/

			/*if (traficjam == 1) {
				if (msg.current == FORWARD_ACTION) {
					if (bEnableDirection[1] == true) {
						mmdagent->sendMessage("渋滞", "%s", "LEFT_OK");
					}
					else if (bEnableDirection[2] == true) {
						mmdagent->sendMessage("渋滞", "%s", "RIGHT_OK");
					}
					else {
						mmdagent->sendMessage("渋滞", "%s", "NO");
					}
				}
				else if (msg.current == LEFT_TURN_ACTION) {
					if (bEnableDirection[0] == true) {
						mmdagent->sendMessage("渋滞", "%s", "FORWARD_OK");
					}
					else if (bEnableDirection[2] == true) {
						mmdagent->sendMessage("渋滞", "%s", "RIGHT_OK");
					}
					else {
						mmdagent->sendMessage("渋滞", "%s", "NO");
					}
				}
				else if (msg.current == RIGHT_TURN_ACTION) {
					if (bEnableDirection[0] == true) {
						mmdagent->sendMessage("渋滞", "%s", "FORWARD_OK");
					}
					else if (bEnableDirection[1] == true) {
						mmdagent->sendMessage("渋滞", "%s", "LEFT_OK");
					}
					else {
						mmdagent->sendMessage("渋滞", "%s", "NO");
					}
				}
				traficjam = 0;
			}*/


			//今の値を、前の値として保存
			for (int i = 0; i < DIRECTION_NUM; i++) {
				pre_bEnableDirection[i] = bEnableDirection[i];
			}

			//変わっている場合の処理
			if (flag_EnableDirectionChange) {
				// ●●FSTで受ける形で出力させる
				m_Comm.m_LatestClientMsg = "Message>" + std::to_string(cnt) + " " + m_Comm.m_LatestClientMsg;
				mmdagent->sendMessage("VEHICLE_RECEIVE", "%s", LPCTSTR(m_Comm.m_LatestClientMsg.c_str()));
			}

			/*
			if(渋滞情報が来たら){
			mmdagent->sendMessage("traffic_situation", "%s", "traffic_jam");
			//現在、渋滞で進路変更できるかの確認のために適当に進路を変更しているので後で変更する
			}
			*/


			//----- 動く方向チェック（オプション読み込み） ------
			//----- 動ける方向が変わったときの処理 ------
			//前の値との比較
			/*if (PreviousSelection != msg.current || PreviousCurrID != msg.currID) {
			//変わっている場合の処理
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

			//----- 動く方向チェック（オプション読み込み） ------
			//----- 動ける方向が変わったときの処理 ------
			//前の値との比較
			if (PreviousSelection != msg.current) {
				//変わっている場合の処理
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

			//今の値を、前の値として保存
			PreviousSelection = msg.current;
			PreviousCurrID = msg.currID;

		}
	}
}