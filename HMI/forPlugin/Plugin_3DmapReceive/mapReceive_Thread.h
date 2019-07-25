#pragma once
#ifndef __3DmapReceive_Thread_h__
#define __3DmapReceive_Thread_h__

#include "MMDAgent.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

class ExamplePacketListener : public osc::OscPacketListener {
private:

	MMDAgent *m_mmdagent;
	GLFWmutex m_mutex;
	GLFWthread m_thread; /* thread */

	/* initialize: initialize thread */
	void initialize();

	/* clear: free thread */
	void clear();

public:

	ExamplePacketListener();
	~ExamplePacketListener();
	void loadAndStart(MMDAgent *m_mmdagent);
	void run();
    virtual void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint);
};

#endif /*__3DmapReceive_thread__*/