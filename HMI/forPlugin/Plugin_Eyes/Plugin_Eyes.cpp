#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINEYES_NAME "Eyes"


/* headers */

#include "MMDAgent.h"
#include "Eyes_Thread.h"

/* variables */
static bool enable;

static Eyes_Thread eyes;

//static Leap_Thread leap;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
	eyes.loadAndStart(mmdagent);
	enable = true;
	mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINEYES_NAME);
	//char passbuf[] = "DataReceiverTest.exe";
	//system(passbuf);
	mmdagent->sendMessage("pointer", "%p", &mmdagent);
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{

}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{

}

/* extUpdate: update log view */
EXPORT void extUpdate(MMDAgent *mmdagent, double frame)
{

}

/* extRender: render log view when debug display mode */
EXPORT void extRender(MMDAgent *mmdagent)
{

}