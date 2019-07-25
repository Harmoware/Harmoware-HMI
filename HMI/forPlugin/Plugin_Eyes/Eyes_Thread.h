//#include "stdafx.h"
//#include "DataReceiverTest.h"
//#include "DataReceiverTestDlg.h"
#include "MMDAgent.h"

class Eyes_Thread
{
private:

	MMDAgent *m_mmdagent;
	//CDataReceiverTestDlg *bb;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /*thread*/
	void *m_pMappingView;
	HANDLE sisen;

	/* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	/* Multi_Thread: thread constructor */
	Eyes_Thread();

	/* ~Julius_Thread: thread destructor  */
	~Eyes_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);

	/* run: main loop */
	void run();
};