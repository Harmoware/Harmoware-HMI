#pragma execution_character_set("utf-8")

#include "MMDAgent.h"
#include "map_Thread.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

//#define OSC_ADDRESS "127.0.0.1"
#define OSC_ADDRESS "192.168.3.15"
#define OSC_PORT 7000
#define OSC_OUTPUT_BUFFER_SIZE 1024

static void mainThread(void *param)
{
	Map_Thread *map = (Map_Thread *)param;
	map->run();
}


void Map_Thread::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	/* create leapmotion thread */
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);
}

void Map_Thread::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;
}

void Map_Thread::clear()
{
}

Map_Thread::Map_Thread()
{
	initialize();
}

Map_Thread::~Map_Thread()
{
	clear();
}

void Map_Thread::map_event(MMDAgent *mmdagent, const char *str) {

	UdpTransmitSocket transmitSocket( IpEndpointName( OSC_ADDRESS, OSC_PORT));
	char buffer[OSC_OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, OSC_OUTPUT_BUFFER_SIZE);

	p << osc::BeginBundleImmediate
		<< osc::BeginMessage("/req")
			<< (int)mmdagent->face_tracking
			<< (float)mmdagent->face_yaw
			<< (float)mmdagent->face_pitch
			<< (float)mmdagent->face_roll
			<< (float)mmdagent->face_pos_x
			<< (float)mmdagent->face_pos_y
			<< (float)mmdagent->face_pos_z
			<< (float)mmdagent->eye_open
			<< (int)mmdagent->gaze_detect
			<< (float)mmdagent->gaze_horizontal
			<< (float)mmdagent->gaze_vertical
		<< osc::EndMessage
		<< osc::EndBundle;

	transmitSocket.Send(p.Data(), p.Size());
	//mmdagent->sendMessage("í êM", "%s", "ëóêM");
}

void Map_Thread::run()
{
	MMDAgent *mmdagent;
	mmdagent = m_mmdagent;

	while (1) {
	}
}