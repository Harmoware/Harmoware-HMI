#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGIN3DMAP_NAME "3Dmap"
#define PLUGIN3DMAP_STARTCOMMAND "MAP_START"

/* headers */

#include "MMDAgent.h"
#include "map_Thread.h"


/* variables */
static bool enable;

static Map_Thread map;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
	map.loadAndStart(mmdagent);
	enable = true;
	mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGIN3DMAP_NAME);
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{
	if (MMDAgent_strequal(type, PLUGIN3DMAP_STARTCOMMAND)) {
		map.map_event(mmdagent, args);
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
