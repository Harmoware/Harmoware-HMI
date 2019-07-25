//#include "afxwin.h"
#include "MMDAgent.h"
#include"CommToAuto.h"


class HMI_Thread
{

protected:
	CommToAuto m_Comm;

private:

	MMDAgent *m_mmdagent;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /*thread*/
	bool turn_right_enable;  //�����_�ŉE�ɋȂ����ꍇ��true, �Ȃ���Ȃ��ꍇfalse
	bool turn_left_enable;   //�����_�ō��ɋȂ����ꍇ��true, �Ȃ���Ȃ��ꍇfalse
	bool go_straight_enable; //�����_�Œ��i�ł���ꍇ��true, �ł��Ȃ��ꍇfalse

						 /* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	/* Multi_Thread: thread constructor */
	HMI_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);
	void vehicle_event(MMDAgent *mmdagent, const char *str);
	void vehicle_stop_event(MMDAgent *mmdagent, const char *str);

	/* run: main loop */
	void run();
};