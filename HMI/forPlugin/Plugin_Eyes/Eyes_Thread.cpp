#pragma execution_character_set("utf-8")

#include "stdafx.h"
#include "Eyes_Thread.h"
#include "DataReceiverTest.h"
#include "DataReceiverTestDlg.h"
//#include "MMDAgent.h"

CDataReceiverTestDlg *bb;

//共有メモリ 用構造体
struct Datas
{
	int shared_tracking_mode;
	double shared_face_dir_yaw;
	double shared_face_dir_pitch;
	double shared_face_dir_roll;
	double shared_face_pos_x;
	double shared_face_pos_y;
	double shared_face_pos_z;
	double shared_eye_opening;
	int shared_long_eye_closure;
	int shared_gaze_detection;
	double shared_gaze_horizontal;
	double shared_gaze_vertical;
	int shared_attention_point;
	int shared_nodding;
	int shared_swing;
	double shared_VVAD_score;
	int shared_VVAD;
};
struct Datas *Alldata;
struct Datas Nulldata;

static void mainThread(void *param)
{
	Eyes_Thread *eyes = (Eyes_Thread *)param;
	eyes->run();
}

void Eyes_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/*create Multi thread*/
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);
}

void Eyes_Thread::initialize()
{
	m_mmdagent = NULL;
	bb = NULL;
	m_mutex = NULL;
	m_thread = -1;

	//初期化で適当な値を入れる　共有メモリ
	/*Alldata = &Nulldata;
	Alldata->shared_tracking_mode = -999;
	Alldata->shared_face_dir_pitch = -999;
	Alldata->shared_face_dir_roll = -999;
	Alldata->shared_face_dir_yaw = -999;
	Alldata->shared_face_pos_x = -999;
	Alldata->shared_face_pos_y = -999;
	Alldata->shared_face_pos_z = -999;
	Alldata->shared_eye_opening = -999;
	Alldata->shared_long_eye_closure = -999;
	Alldata->shared_gaze_detection = -999;
	Alldata->shared_gaze_horizontal = -999;
	Alldata->shared_gaze_vertical = -999;
	Alldata->shared_attention_point = -999;
	Alldata->shared_nodding = -999;
	Alldata->shared_swing = -999;
	Alldata->shared_VVAD_score = -999;
	Alldata->shared_VVAD = -999;
	memcpy(m_pMappingView, Alldata, sizeof(Datas));*/
}

void Eyes_Thread::clear()
{
	//共有メモリ　終了処理
	BOOL b = ::UnmapViewOfFile(m_pMappingView);
	::CloseHandle(sisen);
}

Eyes_Thread::Eyes_Thread()
{
	initialize();
}

Eyes_Thread::~Eyes_Thread()
{
	clear();
}


void Eyes_Thread::run()
{
	//char passbuf1[] = "sisendata\\02_Source\\Binary\\AI_DMS.exe";
	//system(passbuf1);
	//char passbuf2[] = "DataReceiverTest.exe";
	//system(passbuf2);

	int current_attention_point = 0;
	int pre_attention_point = 0;
	int state = 0;

	sisen = ::CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, 4096, "SharedMemory");
	//sisen = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "SharedMemory");
	m_pMappingView = ::MapViewOfFile(sisen, FILE_MAP_ALL_ACCESS, 0, 0, 4096);

	while (1) {
		Alldata = (Datas(*))m_pMappingView;

		m_mmdagent->face_tracking = Alldata->shared_tracking_mode;
		m_mmdagent->face_yaw = Alldata->shared_face_dir_yaw;
		m_mmdagent->face_pitch = Alldata->shared_face_dir_pitch;
		m_mmdagent->face_roll = Alldata->shared_face_dir_roll;
		m_mmdagent->face_pos_x = Alldata->shared_face_pos_x;
		m_mmdagent->face_pos_y = Alldata->shared_face_pos_y;
		m_mmdagent->face_pos_z = Alldata->shared_face_pos_z;
		m_mmdagent->eye_open = Alldata->shared_eye_opening;
		m_mmdagent->eye_closure = Alldata->shared_long_eye_closure;
		m_mmdagent->gaze_detect = Alldata->shared_gaze_detection;
		m_mmdagent->gaze_horizontal = Alldata->shared_gaze_horizontal;
		m_mmdagent->gaze_vertical = Alldata->shared_gaze_vertical;
		m_mmdagent->eye_gaze_point = Alldata->shared_attention_point;
		m_mmdagent->eye_gaze_nod    = Alldata->shared_nodding;
		m_mmdagent->eye_gaze_swing  = Alldata->shared_swing;
		m_mmdagent->VVAD            = Alldata->shared_VVAD;
		m_mmdagent->VVAD_score      = Alldata->shared_VVAD_score;
		
		

	}
}