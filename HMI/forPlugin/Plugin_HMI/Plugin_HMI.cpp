#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINHMI_NAME "HMI"
#define PLUGINVEHICLE_STARTCOMMAND "VEHICLE_START"
#define PLUGINVEHICLE_STOPCOMMAND "EYE_STOP"

/* headers */

#include "MMDAgent.h"
#include "HMI_Thread.h"

/* variables */
static bool enable;

static HMI_Thread hmi;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
	hmi.loadAndStart(mmdagent);
	enable = true;
	mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINHMI_NAME);
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{
	if (MMDAgent_strequal(type, PLUGINVEHICLE_STARTCOMMAND)) {
		hmi.vehicle_event(mmdagent, args);
	}
	else if (MMDAgent_strequal(type, PLUGINVEHICLE_STOPCOMMAND)) {
		hmi.vehicle_stop_event(mmdagent, args);
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
