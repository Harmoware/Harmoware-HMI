#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINMULTI_NAME "Multimodal"


/* headers */

#include "MMDAgent.h"
#include "Multi_Thread.h"

/* variables */
static bool enable;

static Multi_Thread multi;

//static Leap_Thread leap;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
	multi.loadAndStart(mmdagent);
	enable = true;
	mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINMULTI_NAME);
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{
	if (MMDAgent_strequal(type, "yubi_flag")) {
		multi.set_flag();
		mmdagent->sendMessage("yubi_Flag", "%s", "Stand");
	}
	else if (MMDAgent_strequal(type, "state_flag")) {
		multi.set_flag2();
		mmdagent->sendMessage("state_Flag", "%s", "Stand");
	}
	else if (MMDAgent_strequal(type, "VAD_OK")) {
		multi.vad_julius_event(mmdagent, args);
	}
	else if (MMDAgent_strequal(type, "START_MUSIC")) {
		multi.music_event();
	}
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