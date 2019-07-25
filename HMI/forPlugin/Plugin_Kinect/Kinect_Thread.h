#pragma once
#ifndef __kinect_Thread_h__
#define __kinect_Thread_h__


#include "MMDAgent.h"
#include "Kinect.h"

class Kinect_Thread;

class Kinect_Thread {

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
	Kinect_Thread();
	~Kinect_Thread();

	void loadAndStart(MMDAgent *m_mmdagent);

	// •`‰æˆ—

	void run();
};

#endif /*__kinect_thread__*/