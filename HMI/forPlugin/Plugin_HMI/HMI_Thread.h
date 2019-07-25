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
	bool turn_right_enable;  //交差点で右に曲がれる場合はtrue, 曲がれない場合false
	bool turn_left_enable;   //交差点で左に曲がれる場合はtrue, 曲がれない場合false
	bool go_straight_enable; //交差点で直進できる場合はtrue, できない場合false

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