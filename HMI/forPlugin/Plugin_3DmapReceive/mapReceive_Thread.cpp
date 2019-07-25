#pragma execution_character_set("utf-8")

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>

#if defined(__BORLANDC__) // workaround for BCB4 release build intrinsics bug
namespace std {
	using ::__strcmp__;  // avoid error: E2316 '__strcmp__' is not a member of 'std'.
}
#endif

#include "MMDAgent.h"
#include "mapReceive_Thread.h"

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#define OSC_PORT 8000 //石黒先生プログラム用
//#define OSC_PORT 7000 //SimpleSend確認用

#define OSC_OUTPUT_BUFFER_SIZE 1024


static void mainThread(void *param)
{
	ExamplePacketListener *packet = (ExamplePacketListener *)param;
	packet->run();
}

void ExamplePacketListener::ProcessMessage(const osc::ReceivedMessage& m,
	const IpEndpointName& remoteEndpoint)
{

	(void)remoteEndpoint; // suppress unused parameter warning

	try {
		// example of parsing single messages. osc::OsckPacketListener
		// handles the bundle traversal.
		if (strcmp(m.AddressPattern(), "/test1") == 0) { //test用
			// example #1 -- argument stream interface
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			bool a1;
			osc::int32 a2;
			float a3;
			const char *a4;
			args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;

			if (m_mmdagent->building_info == 0) {
				m_mmdagent->sendMessage("building", "%s", a4);
			}
			else if (m_mmdagent->building_info == 1) {
				m_mmdagent->sendMessage("building_info", "%s", a4);
			}
			m_mmdagent->building_info = 0;
			//m_mmdagent->building = MMDAgent_strdup(a4);
			//m_mmdagent->sendMessage("通信", "%s", m_mmdagent->building);
			//MessageBox(NULL, "Message Box", a4, MB_OK);
			
		}

		else if (strcmp(m.AddressPattern(), "/building") == 0) { //3Dmapシステム用_"/buildingは3Dmapシステムと合わせておく"
			// example #1 -- argument stream interface
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			const char *a1;
			args >> a1 >> osc::EndMessage;  //a1には3Dmapシステムから送られてくる建物名が入る

			//MessageBox(NULL, "Message Box を表示", a1, MB_OK);
			//m_mmdagent->building = MMDAgent_strdup(a1);
			if (m_mmdagent->building_info == 0) {
				m_mmdagent->sendMessage("building", "%s", a1);
			}
			else if (m_mmdagent->building_info == 1) {
				m_mmdagent->sendMessage("building_info", "%s", a1);
			}
			m_mmdagent->building_info = 0;


		}

	}
	catch (osc::Exception& e) {
		// any parsing errors such as unexpected argument types, or 
		// missing arguments get thrown as exceptions.
		//std::cout << "error while parsing message: "
		//<< m.AddressPattern() << ": " << e.what() << "\n";
	}
}

void ExamplePacketListener::loadAndStart(MMDAgent *mmdagent)
{
	m_mmdagent = mmdagent;
	glfwInit();
	m_mutex = glfwCreateMutex();
	m_thread = glfwCreateThread(mainThread, this);
}

void ExamplePacketListener::initialize()
{
	m_mmdagent = NULL;
	m_mutex = NULL;
	m_thread = -1;
}

void ExamplePacketListener::clear()
{
	initialize();
}

ExamplePacketListener::ExamplePacketListener()
{
	initialize();
}

ExamplePacketListener::~ExamplePacketListener()
{
	clear();
}

void ExamplePacketListener::run()
{

	UdpListeningReceiveSocket s(
		IpEndpointName(IpEndpointName::ANY_ADDRESS, OSC_PORT),
		this);

	s.RunUntilSigInt();
}