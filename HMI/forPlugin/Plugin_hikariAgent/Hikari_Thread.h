#pragma once
#ifndef __hikari_Thread_h__
#define __hikari_Thread_h__

#include "MMDAgent.h"

class Hikari_Thread {

private:

	MMDAgent *m_mmdagent;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /* thread */

						 /* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	/* LeapApp: thread constructor */
	Hikari_Thread();
	~Hikari_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);
	void hikari_event(MMDAgent *mmdagent, const char *str);
	void run();
};

#endif /*__hikari_thread__*/