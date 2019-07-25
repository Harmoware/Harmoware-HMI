#pragma execution_character_set("utf-8")

#include "MMDAgent.h"
#include "Hikari_Thread.h"
#include "windows.h"

HANDLE arduino;
bool Ret;
DCB dcb;
DWORD numberOfPut;
DWORD lengthOfSent = 0;

static void mainThread(void *param)
{
	Hikari_Thread *hikari = (Hikari_Thread *)param;
	hikari->run();
}


void Hikari_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/* create leapmotion thread */
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);

	arduino = CreateFile("COM3", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (arduino == INVALID_HANDLE_VALUE)
	{
		mmdagent->sendMessage("Hikari open error", "%s", " hikari hirakenai!");
		//exit(0);
	}

	SetupComm(arduino, 1024, 1024);

	PurgeComm(arduino, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	GetCommState(arduino, &dcb);
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 9600;
	dcb.fBinary = TRUE;
	dcb.ByteSize = 8;
	//dcb.Parity = NOPARITY;
	dcb.fParity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;

	SetCommState(arduino, &dcb);

	/*現在一回目でarduinoが反応しないので初期化でメッセージを一回送る*/
	lengthOfSent = strlen("#2,200,0,0\n");
	WriteFile(arduino, "#2,200,0,0\n", lengthOfSent, &numberOfPut, NULL);
	/*一回目で反応するようになればこれ↑を消す*/


	//Sleep(5000);

}

void Hikari_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;
}

void Hikari_Thread::clear()
{
	lengthOfSent = strlen("#-1\n");
	WriteFile(arduino, "#-1\n", lengthOfSent, &numberOfPut, NULL);
	CloseHandle(arduino);
}

Hikari_Thread::Hikari_Thread()
{
	initialize();
}

Hikari_Thread::~Hikari_Thread()
{
	clear();
}

void Hikari_Thread::hikari_event(MMDAgent *mmdagent, const char *str)
{
	mmdagent->sendMessage("Hikari debug message(str)", "%s", str);

	if (strcmp(str, "赤") == 0) {
		//Sleep(100);
		lengthOfSent = strlen("#2,200,0,0\n");
		WriteFile(arduino, "#2,200,0,0\n", lengthOfSent, &numberOfPut, NULL);
	}
	else if (strcmp(str, "ピンク") == 0) {
		//Sleep(100);
		lengthOfSent = strlen("#6,0,150,200,50,50,0\n");
		WriteFile(arduino, "#6,0,150,200,50,50,0\n", lengthOfSent, &numberOfPut, NULL);
	}
	else if (strcmp(str, "アップ") == 0) {
		//Sleep(100);
		lengthOfSent = strlen("#7,100,32\n");
		WriteFile(arduino, "#7,100,32\n", lengthOfSent, &numberOfPut, NULL);
	}
	else if (strcmp(str, "音声開始") == 0) {
		lengthOfSent = strlen("#3,50,32\n");
		WriteFile(arduino, "#3,50,32\n", lengthOfSent, &numberOfPut, NULL);
	}
	else if (strcmp(str, "音声終了") == 0) {
		lengthOfSent = strlen("#2,0,0,32\n");//255
		WriteFile(arduino, "#2,0,0,32\n", lengthOfSent, &numberOfPut, NULL);
		//Sleep(100);
		//lengthOfSent = strlen("#-1\n");
		//WriteFile(arduino, "#-1\n", lengthOfSent, &numberOfPut, NULL);
	}
	else if (strcmp(str, "終了") == 0) {
		lengthOfSent = strlen("#-1\n");
		WriteFile(arduino, "#-1\n", lengthOfSent, &numberOfPut, NULL);
		CloseHandle(arduino);
	}
}

void Hikari_Thread::run()
{

}