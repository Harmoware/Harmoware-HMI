#pragma once
#ifndef __3Dmap_Thread_h__
#define __3Dmap_Thread_h__

#include "MMDAgent.h"

class Map_Thread {

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
	Map_Thread();
	~Map_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);
	void run();
	void map_event(MMDAgent *mmdagent, const char *str);
};

#endif /*__3Dmap_thread__*/