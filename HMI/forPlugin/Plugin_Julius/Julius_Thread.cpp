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

/* headers */

#include "MMDAgent.h"
#include "julius/juliuslib.h"
#include "Julius_Logger.h"
#include "Julius_Thread.h"

FILETIME hajime, owari;
double starttime, endtime, kekkatime;
int suuji, vad_flag;

/* callbackRecogBegin: callback for beginning of recognition */
static void callbackRecogBegin(Recog *recog, void *data)
{
   Julius_Thread *j = (Julius_Thread *) data;
   j->sendMessage(JULIUSTHREAD_EVENTSTART, "");
   vad_flag = 1;
   j->getjulius_vad_flag(vad_flag);
   suuji = 1;
   j->getjulius_suuji(suuji);
   GetSystemTimeAsFileTime(&hajime);
   starttime = (hajime.dwHighDateTime / 10000000.0) * 0x10000 * 0x10000 + hajime.dwLowDateTime / 10000000.0;
}

/* callbackRecogResult: callback for recognition result */
static void callbackRecogResult(Recog *recog, void *data)
{
   Julius_Thread *j = (Julius_Thread *) data;
   j->procResult();
}

/* callbackPoll: callback called per about 0.1 sec during audio input */
static void callbackPoll(Recog *recog, void *data)
{
   Julius_Thread *j = (Julius_Thread *) data;
   j->procCommand();
}

/* callbackOnLine: callback for start of processing */
static void callbackOnLine(Recog *recog, void *data)
{
   Julius_Thread *j = (Julius_Thread *) data;
   j->sendMessage(MMDAGENT_EVENT_PLUGINENABLE, JULIUSTHREAD_PLUGINNAME);
}

/* mainThread: main thread */
static void mainThread(void *param)
{
   Julius_Thread *julius_thread = (Julius_Thread *) param;
   julius_thread->run();
}

/* Julius_Thread::initialize: initialize thread */
void Julius_Thread::initialize()
{
   m_jconf = NULL;
   m_recog = NULL;

   m_mmdagent = NULL;

   m_mutex = NULL;
   m_thread = -1;

   m_configFile = NULL;
   m_userDictionary = NULL;

   m_status = JULIUSTHREAD_STATUSWAIT;
   m_command = NULL;
}

/* Julius_Thread::clear: free thread */
void Julius_Thread::clear()
{
   JuliusModificationCommand *c, *next;

   if(m_thread >= 0) {
      if(m_recog)
         j_close_stream(m_recog);
      glfwWaitThread(m_thread, GLFW_WAIT);
      glfwDestroyThread(m_thread);
   }
   if(m_mutex != NULL)
      glfwDestroyMutex(m_mutex);
   if (m_recog)
      j_recog_free(m_recog); /* jconf is also released in j_recog_free */
   else if (m_jconf)
      j_jconf_free(m_jconf);

   if(m_userDictionary != NULL)
      free(m_userDictionary);
   if(m_configFile != NULL)
      free(m_configFile);

   for(c = m_command; c != NULL; c = next) {
      next = c->next;
      free(c->str);
      free(c);
   }

   initialize();
}

/* Julius_Thread::Julius_Thread: thread constructor */
Julius_Thread::Julius_Thread()
{
   initialize();
}

/* Julius_Thread::~Julius_Thread: thread destructor */
Julius_Thread::~Julius_Thread()
{
   clear();
}

/* Julius_Thread::loadAndStart: load models and start thread */
void Julius_Thread::loadAndStart(MMDAgent *mmdagent, const char *configFile, const char *userDictionary)
{
   /* reset */
   clear();

   m_mmdagent = mmdagent;

   m_configFile = MMDAgent_strdup(configFile);
   m_userDictionary = MMDAgent_strdup(userDictionary);

   if(m_mmdagent == NULL || m_configFile == NULL) {
      clear();
      return;
   }

   /* create recognition thread */
   glfwInit();
   m_mutex = glfwCreateMutex();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_mutex == NULL || m_thread < 0) {
      clear();
      return;
   }
}

/* Julius_Thread::stopAndRelease: stop thread and release julius */
void Julius_Thread::stopAndRelease()
{
   clear();
}

/* Julius_Thread::run: main loop */
void Julius_Thread::run()
{
   char *tmp;
   char buff[MMDAGENT_MAXBUFLEN];
   FILE *fp;

   if(m_jconf != NULL || m_recog != NULL || m_mmdagent == NULL || m_thread < 0 || m_configFile == NULL)
      return;

   /* set latency */
#if !defined(__APPLE__)
   sprintf(buff, "PA_MIN_LATENCY_MSEC=%d", JULIUSTHREAD_LATENCY);
   putenv(buff);
   sprintf(buff, "LATENCY_MSEC=%d", JULIUSTHREAD_LATENCY);
   putenv(buff);
#endif

   /* turn off log */
   jlog_set_output(NULL);

   /* load config file */
   tmp = MMDAgent_pathdup_from_application_to_system_locale(m_configFile);
   m_jconf = j_config_load_file_new(tmp);
   if (m_jconf == NULL) {
      free(tmp);
      return;
   }
   free(tmp);

   /* load user dictionary */
   if (MMDAgent_strlen(m_userDictionary) > 0) {
      fp = MMDAgent_fopen(m_userDictionary, "r");
      if (fp != NULL) {
         fclose(fp);
         tmp = MMDAgent_pathdup_from_application_to_system_locale(m_userDictionary);
         j_add_dict(m_jconf->lm_root, tmp);
         free(tmp);
      }
   }

   /* create instance */
   m_recog = j_create_instance_from_jconf(m_jconf);
   if (m_recog == NULL) {
      j_jconf_free(m_jconf);
      m_jconf = NULL;
      return;
   }

   /* register callback functions */
   callback_add(m_recog, CALLBACK_EVENT_RECOGNITION_BEGIN, callbackRecogBegin, this);
   callback_add(m_recog, CALLBACK_RESULT, callbackRecogResult, this);
   callback_add(m_recog, CALLBACK_POLL, callbackPoll, this);
   callback_add(m_recog, CALLBACK_EVENT_PROCESS_ONLINE, callbackOnLine, this);

   /* open audio device */
   if (!j_adin_init(m_recog)) {
      j_recog_free(m_recog);
      m_recog = NULL;
      m_jconf = NULL;
      return;
   }

   if (j_open_stream(m_recog, NULL) != 0) {
      j_recog_free(m_recog);
      m_recog = NULL;
      m_jconf = NULL;
      return;
   }

   /* setup logger */
   m_logger.setup(m_recog);

   /* start logger */
   m_logger.setActiveFlag(true);

   /* start recognize */
   j_recognize_stream(m_recog);
}

/* Julius_Thread::pause: pause recognition process */
void Julius_Thread::pause()
{
   if(m_recog != NULL)
      j_adin_change_input_scaling_factor(m_recog, 0.0f);
}

/* Julius_Thread::resume: resume recognition process */
void Julius_Thread::resume()
{
   if(m_recog != NULL)
      j_adin_change_input_scaling_factor(m_recog, 1.0f);
}

/* Julius_Thread::procResult: process recognition result */
void Julius_Thread::procResult()
{
   int i;
   int first;
   Sentence *sentence;
   RecogProcess *process;
   char result[MMDAGENT_MAXBUFLEN];
   float maxScore;
   RecogProcess *processWithMaxScore;
   char buff[MMDAGENT_MAXBUFLEN]; /*”­˜bŽž•\Ž¦ buffer*/

   maxScore = LOG_ZERO;
   processWithMaxScore = NULL;
   for (process = m_recog->process_list; process; process = process->next) {
      if (!process->live)
         continue;
      if (process->result.status < 0)
         continue;
      sentence = &(process->result.sent[0]);
      if (maxScore < sentence->score_am) {
         maxScore = sentence->score_am;
         processWithMaxScore = process;
      }
   }

   strcpy(result, "");
   if (processWithMaxScore != NULL) {
      process = processWithMaxScore;
      sentence = &(process->result.sent[0]);
      first = 1;
      for (i = 0; i < sentence->word_num; i++) {
         if (MMDAgent_strlen(process->lm->winfo->woutput[sentence->word[i]]) > 0) {
            if(first == 1) {
               first = 0;
            } else {
               strcat(result, ",");
            }
            strcat(result, process->lm->winfo->woutput[sentence->word[i]]);
         }
      }

	  if (first == 0) {
		  sendMessage(JULIUSTHREAD_EVENTSTOP, result);
		  vad_flag = 0;
		  getjulius_vad_flag(vad_flag);
		  //suuji = 2;
		  getjulius_suuji(suuji);
		  getJulius(result);

		  GetSystemTimeAsFileTime(&owari);
		  endtime = (owari.dwHighDateTime / 10000000.0) * 0x10000 * 0x10000 + owari.dwLowDateTime / 10000000.0;
		  kekkatime = endtime - starttime;
		  sprintf(buff, "%.10f", endtime);
		  getJulius_time(endtime);
		  //sendMessage("Utterance time interval", buff);
	  }
   }
   else {
	   strcpy(result, "");
	   sendMessage(JULIUSTHREAD_EVENTSTOP, result);
	   vad_flag = 0;
	   getjulius_vad_flag(vad_flag);
	   suuji = 2;
	   getjulius_suuji(suuji);
   }
}

/* Julius_Thread::procCommand: process command message to modify recognition condition */
void Julius_Thread::procCommand()
{
   JuliusModificationCommand *c;
   char *p1, *p2, *save;
   float f;
   PROCESS_LM *lm;
   JCONF_LM_NAMELIST *dict, *next;

   if(m_recog != NULL) {
      while(1) {
         /* dequeue command message */
         glfwLockMutex(m_mutex);
         c = m_command;
         if(c != NULL)
            m_command = c->next;
         glfwUnlockMutex(m_mutex);
         if(c == NULL)
            return;
         /* change status */
         while(1) {
            glfwLockMutex(m_mutex);
            if(m_status == JULIUSTHREAD_STATUSWAIT) {
               m_status = JULIUSTHREAD_STATUSMODIFY;
               glfwUnlockMutex(m_mutex);
               break;
            }
            glfwUnlockMutex(m_mutex);
            MMDAgent_sleep(0.1);
         }
         /* process command message */
         p1 = MMDAgent_strtok(c->str, "|", &save);
         p2 = MMDAgent_strtok(NULL, "|", &save);
         if(MMDAgent_strequal(p1, JULIUSTHREAD_GAIN)) {
            f = (float) atof(p2);
            if(f > 1.0)
               f = 1.0;
            else if(f < 0.0)
               f = 0.0;
            j_adin_change_input_scaling_factor(m_recog, f);
            this->sendMessage(JULIUSTHREAD_EVENTMODIFY, JULIUSTHREAD_GAIN);
         } else if(MMDAgent_strequal(p1, JULIUSTHREAD_USERDICTSET)) {
            if(MMDAgent_strlen(p2) > 0) {
               for(lm = m_recog->lmlist; lm; lm = lm->next) {
                  /* free all additional dictionary names */
                  for(dict = lm->config->additional_dict_files; dict; dict = next) {
                     next = dict->next;
                     if(dict->name)
                        free(dict->name);
                     free(dict);
                  }
                  /* set additional dictionary name */
                  lm->config->additional_dict_files = (JCONF_LM_NAMELIST *) malloc(sizeof(JCONF_LM_NAMELIST));
                  lm->config->additional_dict_files->name = MMDAgent_pathdup_from_application_to_system_locale(p2);
                  lm->config->additional_dict_files->next = NULL;
                  /* reload */
                  j_reload_adddict(m_recog, lm);
               }
               this->sendMessage(JULIUSTHREAD_EVENTMODIFY, JULIUSTHREAD_USERDICTSET);
            }
         } else if(MMDAgent_strequal(p1, JULIUSTHREAD_USERDICTUNSET)) {
            for(lm = m_recog->lmlist; lm; lm = lm->next) {
               /* free all additional dictionary names */
               for(dict = lm->config->additional_dict_files; dict; dict = next) {
                  next = dict->next;
                  if(dict->name)
                     free(dict->name);
                  free(dict);
               }
               /* set additional dictionary name */
               lm->config->additional_dict_files = NULL;
               /* reload */
               j_reload_adddict(m_recog, lm);
            }
            this->sendMessage(JULIUSTHREAD_EVENTMODIFY, JULIUSTHREAD_USERDICTUNSET);
         }
         free(c->str);
         free(c);
         /* change status */
         glfwLockMutex(m_mutex);
         m_status = JULIUSTHREAD_STATUSWAIT;
         glfwUnlockMutex(m_mutex);
      }
   }
}

/* Julius_Thread::storeCommand: store command message to modify recognition condition */
void Julius_Thread::storeCommand(const char *args)
{
   JuliusModificationCommand *c1, *c2;

   if(m_recog != NULL && MMDAgent_strlen(args) > 0) {
      /* create command message */
      c1 = (JuliusModificationCommand *) malloc(sizeof(JuliusModificationCommand));
      c1->str = MMDAgent_strdup(args);
      c1->next = NULL;
      /* store command message */
      glfwLockMutex(m_mutex);
      if(m_command == NULL)
         m_command = c1;
      else {
         for(c2 = m_command; c2->next != NULL; c2 = c2->next);
         c2->next = c1;
      }
      glfwUnlockMutex(m_mutex);
   }
}

/* Julius_Thread::sendMessage: send message to MMDAgent */
void Julius_Thread::sendMessage(const char *str1, const char *str2)
{
   m_mmdagent->sendMessage(str1, "%s", str2);
}


char *Julius_Thread::getJulius(char *str) {
	m_mmdagent->julius_result = MMDAgent_strdup(str);
	return m_mmdagent->julius_result;
}

double Julius_Thread::getJulius_time(double time) {
	m_mmdagent->julius_nowtime = time;
	return m_mmdagent->julius_nowtime;
}

int Julius_Thread::getjulius_suuji(int suuji) {
	m_mmdagent->julius_suuji = suuji;
	return m_mmdagent->julius_suuji;
}

int Julius_Thread::getjulius_vad_flag(int n) {
	m_mmdagent->julius_VAD_start = n;
	return m_mmdagent->julius_VAD_start;
}

/* Julius_Thread::getLogActiveFlag: get active flag of logger */
bool Julius_Thread::getLogActiveFlag()
{
   return m_logger.getActiveFlag();
}

/* Julius_Thread::setLogActiveFlag: set active flag of logger */
void Julius_Thread::setLogActiveFlag(bool b)
{
   m_logger.setActiveFlag(b);
}

/* Julius_Thread::getLogFlagForFixedLocationInWindow: get flag of logger for fixed location in window */
bool Julius_Thread::getLogFlagForFixedLocationInWindow()
{
   return m_logger.getFlagForFixedLocationInWindow();
}

/* Julius_Thread::setLogFlagForFixedLocationInWindow: set flag of logger for fixed location in window */
void Julius_Thread::setLogFlagForFixedLocationInWindow(bool b)
{
   m_logger.setFlagForFixedLocationInWindow(b);
}

/* Julius_Thread::updateLog: update log view per step */
void Julius_Thread::updateLog(double frame)
{
   bool updateFlag = false;

   glfwLockMutex(m_mutex);
   if(m_status == JULIUSTHREAD_STATUSWAIT) {
      updateFlag = true;
      m_status = JULIUSTHREAD_STATUSUPDATE;
   }
   glfwUnlockMutex(m_mutex);

   /* if modifying, skip update */
   if(updateFlag == false)
      return;
   m_logger.update(frame);

   glfwLockMutex(m_mutex);
   m_status = JULIUSTHREAD_STATUSWAIT;
   glfwUnlockMutex(m_mutex);
}

/* Julius_Thread::renderLog: render log view */
void Julius_Thread::renderLog()
{
   bool renderFlag = false;

   glfwLockMutex(m_mutex);
   if(m_status == JULIUSTHREAD_STATUSWAIT) {
      renderFlag = true;
      m_status = JULIUSTHREAD_STATUSRENDER;
   }
   glfwUnlockMutex(m_mutex);

   /* if modifying, skip render */
   if(renderFlag == false)
      return;
   m_logger.render();

   glfwLockMutex(m_mutex);
   m_status = JULIUSTHREAD_STATUSWAIT;
   glfwUnlockMutex(m_mutex);
}
