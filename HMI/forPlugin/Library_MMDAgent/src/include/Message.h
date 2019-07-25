/* ----------------------------------------------------------------- */
/*           Toolkit for Building Voice Interaction Systems          */
/*           MMDAgent developed by MMDAgent Project Team             */
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

/* MessageLink: message */
typedef struct _MessageLink {
   char *type;
   char *value;
   struct _MessageLink *next;
} MessageLink;

/* MessageQueue: message list */
typedef struct _MessageQueue {
   MessageLink *head;
   MessageLink *tail;
} MessageQueue;

/* Message: message queue */
class Message
{
private:

   GLFWmutex m_messageMutex;         /* message mutex */
   GLFWmutex m_logStringMutex;       /* log string mutex */

   MessageQueue m_messageQueue;      /* message queue */
   MessageQueue m_logStringQueue;    /* log string queue */

   /* initialize: initialize message queue */
   void initialize();

   /* clear: free message queue */
   void clear();

public:

   /* Message: constructor */
   Message();

   /* Message: destructor */
   ~Message();

   /* setup: setup message queue */
   bool setup();

   /* enqueueMessage: enqueue received message */
   void enqueueMessage(const char *type, const char *value);

   /* enqueueLogString: enqueue log string */
   void enqueueLogString(const char *log);

   /* dqeueueMessage: dequeue received message */
   bool dequeueMessage(char *type, char *value);

   /* dequeueLogString: dequeue log string */
   bool dequeueLogString(char *log);
};
