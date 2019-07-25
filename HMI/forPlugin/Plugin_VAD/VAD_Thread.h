//#include "afxwin.h"
#include "MMDAgent.h"

class VAD_Thread
{
private:

	MMDAgent *m_mmdagent;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /*thread*/

	/* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	/* Multi_Thread: thread constructor */
	VAD_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);

	/* run: main loop */
	void run();
};