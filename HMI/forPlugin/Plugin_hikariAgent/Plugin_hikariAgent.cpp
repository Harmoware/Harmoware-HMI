#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINHIKARI_NAME "hikariAgent"
#define PLUGINHIKARI_STARTCOMMAND "HIKARI_START"

/* headers */

#include "MMDAgent.h"
#include "Hikari_Thread.h"

/* variables */
static bool enable;

static Hikari_Thread hikari;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
	hikari.loadAndStart(mmdagent);
	enable = true;
	mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINHIKARI_NAME);
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{
	if (MMDAgent_strequal(type, PLUGINHIKARI_STARTCOMMAND)) {
		hikari.hikari_event(mmdagent, args);
	}
	if (MMDAgent_strequal(type, "RECOG_EVENT_START")) {
		hikari.hikari_event(mmdagent, "âπê∫äJén");
	}
	if (MMDAgent_strequal(type, "RECOG_EVENT_STOP")) {
		hikari.hikari_event(mmdagent, "âπê∫èIóπ");
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
