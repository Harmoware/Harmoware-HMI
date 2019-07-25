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

#define JULIUSTHREAD_PLUGINNAME    "Julius"
#define JULIUSTHREAD_LATENCY       50
#define JULIUSTHREAD_EVENTSTART    "RECOG_EVENT_START"
#define JULIUSTHREAD_EVENTSTOP     "RECOG_EVENT_STOP"
#define JULIUSTHREAD_EVENTMODIFY   "RECOG_EVENT_MODIFY"
#define JULIUSTHREAD_GAIN          "GAIN"
#define JULIUSTHREAD_USERDICTSET   "USERDICT_SET"
#define JULIUSTHREAD_USERDICTUNSET "USERDICT_UNSET"
#define JULIUSTHREAD_CHANGECONF    "CHANGE_CONF"
#define JULIUSTHREAD_STATUSWAIT    0
#define JULIUSTHREAD_STATUSUPDATE  1
#define JULIUSTHREAD_STATUSRENDER  2
#define JULIUSTHREAD_STATUSMODIFY  3

typedef struct _JuliusModificationCommand {
   char *str;
   struct _JuliusModificationCommand *next;
} JuliusModificationCommand;

/* Julius_Thead: thread for Julius */
class Julius_Thread
{
private :

   Jconf *m_jconf; /* configuration parameter data */
   Recog *m_recog; /* engine instance */

   MMDAgent *m_mmdagent;

   GLFWmutex m_mutex;
   GLFWthread m_thread; /* thread */

   char *m_configFile;
   char *m_userDictionary;

   int m_status;
   JuliusModificationCommand *m_command;

   Julius_Logger m_logger;

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

public :

   /* Julius_Thread: thread constructor */
   Julius_Thread();

   /* ~Julius_Thread: thread destructor  */
   ~Julius_Thread();

   /* loadAndStart: load models and start thread */
   void loadAndStart(MMDAgent *m_mmdagent, const char *configFile, const char *userDictionary);

   /* stopAndRlease: stop thread and release julius */
   void stopAndRelease();

   /* run: main loop */
   void run();

   /* pause: pause recognition process */
   void pause();

   /* resume: resume recognition process */
   void resume();

   /* procResult: process recognition result */
   void procResult();

   /* procCommand: process command message to modify recognition condition */
   void procCommand();

   /* storeCommand: store command message to modify recognition condition */
   void storeCommand(const char *args);

   /* sendMessage: send message to MMDAgent */
   void sendMessage(const char *str1, const char *str2);

   char *getJulius(char *str);

   double getJulius_time(double time);

   int getjulius_suuji(int suuji);

   int getjulius_vad_flag(int n);

   /* getLogActiveFlag: get active flag of logger */
   bool getLogActiveFlag();

   /* setLogActiveFlag: set active flag of logger */
   void setLogActiveFlag(bool b);

   /* getLogFlagForFixedLocationInWindow: get flag of logger for fixed location in window */
   bool getLogFlagForFixedLocationInWindow();

   /* setLogFlagForFixedLocationInWindow: set flag of logger for fixed location in window */
   void setLogFlagForFixedLocationInWindow(bool b);

   /* updateLog: update log view per step */
   void updateLog(double frame);

   /* renderLog: render log view */
   void renderLog();
};
