/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2016  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/* definitions */

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#if TARGET_OS_IPHONE
#define EXPORT
#define extAppStart Plugin_Julius_extAppStart
#define extProcMessage Plugin_Julius_extProcMessage
#define extAppEnd Plugin_Julius_extAppEnd
#define extUpdate Plugin_Julius_extUpdate
#define extRender Plugin_Julius_extRender
#else
#define EXPORT extern "C"
#endif
#endif /* _WIN32 */

#define PLUGINJULIUS_NAME          JULIUSTHREAD_PLUGINNAME
#define PLUGINJULIUS_MODIFYCOMMAND "RECOG_MODIFY"

#if defined(__ANDROID__) || TARGET_OS_IPHONE
#define PLUGINJULIUS_DEFAULTCONF "jconf_ja_JP_for_mobile.txt"
#else
#define PLUGINJULIUS_DEFAULTCONF "jconf.txt"
#endif /* __ANDROID__ || TARGET_OS_IPHONE */

/* headers */

#include "MMDAgent.h"
#include "julius/juliuslib.h"
#include "Julius_Logger.h"
#include "Julius_Thread.h"

/* variables */

static Julius_Thread julius_thread;
static bool enable;

/* menu handler */
static void menuHandler(int id, int item, void *data)
{
   MMDAgent *mmdagent = (MMDAgent *)data;
   Menu *menu = mmdagent->getMenu();

   if (menu->find("[Julius]") == id) {
      switch (item) {
      case 2:
         /* rotate position */
         if (julius_thread.getLogActiveFlag() == true)
            if (julius_thread.getLogFlagForFixedLocationInWindow() == true)
               julius_thread.setLogActiveFlag(false);
            else
               julius_thread.setLogFlagForFixedLocationInWindow(true);
         else {
            julius_thread.setLogActiveFlag(true);
            julius_thread.setLogFlagForFixedLocationInWindow(false);
         }
         break;
      }
   }
}

/* createMenu: create menu */
static void createMenu(MMDAgent *mmdagent)
{
   int id;
   Menu *menu;

   if (mmdagent == NULL)
      return;

   menu = mmdagent->getMenu();
   if (menu == NULL)
      return;

   id = menu->add("[Julius]", MENUPRIORITY_SYSTEM, menuHandler, mmdagent);
   menu->setItem(id, 0, "Mute", "RECOG_MODIFY", "GAIN|0");
   menu->setItem(id, 1, "UnMute", "RECOG_MODIFY", "GAIN|1");
   menu->setItem(id, 2, "Level Pos", NULL, NULL);
}

/* startJulius: start Julius */
static void startJulius(MMDAgent *mmdagent, const char *conffile)
{
   int len;
   char configFile[MMDAGENT_MAXBUFLEN];
   char userDictionary[MMDAGENT_MAXBUFLEN];

   /* config file */
   sprintf(configFile, "%s%c%s%c%s", mmdagent->getAppDirName(), MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, conffile);

   /* user dictionary */
   strcpy(userDictionary, mmdagent->getConfigFileName());
   len = MMDAgent_strlen(userDictionary);
   if (len > 4) {
      userDictionary[len - 4] = '.';
      userDictionary[len - 3] = 'd';
      userDictionary[len - 2] = 'i';
      userDictionary[len - 1] = 'c';
   } else {
      strcpy(userDictionary, "");
   }

   /* load models and start thread */
   julius_thread.loadAndStart(mmdagent, configFile, userDictionary);
}

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   /* start Julius */
   startJulius(mmdagent, PLUGINJULIUS_DEFAULTCONF);

   /* create menu */
   createMenu(mmdagent);

   enable = true;
}

/* extProcMessage: process message */
EXPORT void extProcMessage(MMDAgent *mmdagent, const char *type, const char *args)
{
   char *buff;
   char buff2[MMDAGENT_MAXBUFLEN];
   char *p1, *p2, *save;

   if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
      if(MMDAgent_strequal(args, PLUGINJULIUS_NAME) && enable == true) {
         julius_thread.pause();
         enable = false;
         mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINDISABLE, "%s", PLUGINJULIUS_NAME);
      }
   } else if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
      if(MMDAgent_strequal(args, PLUGINJULIUS_NAME) && enable == false) {
         julius_thread.resume();
         enable = true;
         mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINJULIUS_NAME);
      }
   } else if(MMDAgent_strequal(type, MMDAGENT_EVENT_KEY)) {
      if (MMDAgent_strequal(args, "J")) {
         if (julius_thread.getLogActiveFlag() == true)
            if (julius_thread.getLogFlagForFixedLocationInWindow() == true)
               julius_thread.setLogActiveFlag(false);
            else
               julius_thread.setLogFlagForFixedLocationInWindow(true);
         else {
            julius_thread.setLogActiveFlag(true);
            julius_thread.setLogFlagForFixedLocationInWindow(false);
         }
      }
   } else if(enable == true && MMDAgent_strequal(type, PLUGINJULIUS_MODIFYCOMMAND)) {
      buff = MMDAgent_strdup(args);
      p1 = MMDAgent_strtok(buff, "|", &save);
      p2 = MMDAgent_strtok(NULL, "|", &save);
      if (MMDAgent_strequal(p1, JULIUSTHREAD_CHANGECONF)) {
         sprintf(buff2, "%s.txt", p2);
         julius_thread.stopAndRelease();
         mmdagent->sendMessage(MMDAGENT_EVENT_PLUGINDISABLE, "%s", PLUGINJULIUS_NAME);
         startJulius(mmdagent, buff2);
         mmdagent->sendMessage(JULIUSTHREAD_EVENTMODIFY, "%s|%s", JULIUSTHREAD_CHANGECONF, p2);
      } else {
         julius_thread.storeCommand(args);
      }
      free(buff);
   }
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   julius_thread.stopAndRelease();
   enable = false;
}

/* extUpdate: update log view */
EXPORT void extUpdate(MMDAgent *mmdagent, double frame)
{
   julius_thread.updateLog(frame);
}

/* extRender: render log view when debug display mode */
EXPORT void extRender(MMDAgent *mmdagent)
{
   julius_thread.renderLog();
}
