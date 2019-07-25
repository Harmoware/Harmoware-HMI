#pragma execution_character_set("utf-8")

#include "VAD_Thread.h"

static void mainThread(void *param)
{
	VAD_Thread *hmi = (VAD_Thread *)param;
	hmi->run();
}

void VAD_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/*create Multi thread*/
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);

}

void VAD_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;

}

VAD_Thread::VAD_Thread()
{
	initialize();
}

void VAD_Thread::run()
{
	MMDAgent *mmdagent;
	mmdagent = m_mmdagent;
	double speak = 0, not_speak = 0, sum = 0;
	int n = 0;

	while (1) {

		if (mmdagent->julius_VAD_start == 1) {
			n = 1;
			if (mmdagent->VVAD == 1) {
				speak++;
				sum++;
			}
			else if (mmdagent->VVAD == -1) {
				not_speak++;
				sum++;
			}
			else {
				sum++;
			}
		}

		if (mmdagent->julius_VAD_start == 0 && n == 1) {
			if (speak != 0) {
				if (speak / sum > 0.6) {
					mmdagent->sendMessage("ドライバー", "%s", "発見");
					//mmdagent->sendMessage("speak", "%f", speak);
				}
				else {
					mmdagent->sendMessage("ドライバー", "%s", "NG");
					//mmdagent->sendMessage("割合", "%f", speak / sum);
				}
			}
			else {
				mmdagent->sendMessage("ドライバー", "%s", "NG");
				//mmdagent->sendMessage("割合", "%f", speak / sum);
			}
			speak = 0;
			not_speak = 0;
			sum = 0;
			n = 0;
		}
	}
}