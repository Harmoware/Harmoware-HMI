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

/* callbackRecogBegin: callback for beginning of recognition */
static void callbackRecogBegin(Recog *recog, void *data)
{
   Julius_Logger *j = (Julius_Logger *) data;

   j->setRecognitionFlag(true);
}

/* callbackRecogEnd: callback for end of recognition */
static void callbackRecogEnd(Recog *recog, void *data)
{
   Julius_Logger *j = (Julius_Logger *) data;

   j->setRecognitionFlag(false);
   j->startDecay();
}

/* callbackRecogFrame: callback for each input frame */
static void callbackRecogFrame(Recog *recog, void *data)
{
   Julius_Logger *j = (Julius_Logger *) data;

   j->setLastTrellis(recog->process_list->backtrellis->list);
}

/* callbackRcogAdin: callback for input audio segment */
static void callbackRecogAdin(Recog *recog, SP16 *buf, int len, void *data)
{
   Julius_Logger *j = (Julius_Logger *) data;

   j->updateMaxVol(buf, len);
}

/* Julius_Logger::initialize: initialize data */
void Julius_Logger::initialize()
{
   m_active = false;
   m_flagForFixedLocationInWindow = false;
   m_recognizing = false;
   m_lastTrellis = NULL;
   m_decayFrame = 0.0;
   m_currentMaxAdIn = 0;
   m_maxAdIn = 0.0f;
   m_adInFrameStep = 0.0;
   m_numWord = 0;
   m_levelThreshold = 0.0f;
   m_marginScale = 1.0f;

   m_numPos = 0;
   m_numIndex = 0;
}

/* Julius_Logger::clear: clear data */
void Julius_Logger::clear()
{
   initialize();
}

/* Julius_Logger::Julius_Logger: constructor */
Julius_Logger::Julius_Logger()
{
   initialize();
}

/* Julius_Logger::~Julius_Logger: destructor */
Julius_Logger::~Julius_Logger()
{
   clear();
}

/* Julius_Logger::setup: setup for logging */
void Julius_Logger::setup(Recog *recog)
{
   /* reset */
   clear();

   if (recog) {
      /* set callback */
      callback_add(recog, CALLBACK_EVENT_RECOGNITION_BEGIN, callbackRecogBegin, this);
      callback_add(recog, CALLBACK_EVENT_RECOGNITION_END, callbackRecogEnd, this);
      callback_add(recog, CALLBACK_EVENT_PASS1_FRAME, callbackRecogFrame, this);
      callback_add_adin(recog, CALLBACK_ADIN_CAPTURED, callbackRecogAdin, this);
      m_numWord = recog->lmlist->winfo->num;
      m_levelThreshold = (float) recog->jconf->detect.level_thres / 32767.0f;
   }
}

/* setRecognitionFlag: mark recognition start and end */
void Julius_Logger::setRecognitionFlag(bool flag)
{
   m_recognizing = flag;
}

/* setLastTrellis: store the last best word trellis while recognition */
void Julius_Logger::setLastTrellis(TRELLIS_ATOM *atom)
{
   m_lastTrellis = atom;
}

/* updateMaxVol: update maximum volume */
void Julius_Logger::updateMaxVol(SP16 *buf, int len)
{
   int i;

   for (i = 0; i < len; i++) {
      if (m_currentMaxAdIn < abs(buf[i]))
         m_currentMaxAdIn = abs(buf[i]);
   }
}

/* startDecay: start decaying the search status bar */
void Julius_Logger::startDecay()
{
   m_decayFrame = JULIUSLOGGER_DECAYFRAME;
   m_lastTrellis = NULL;
}

/* setActiveFlag: set active flag */
void Julius_Logger::setActiveFlag(bool flag)
{
   m_active = flag;
}

/* getActiveFlag: get active flag */
bool Julius_Logger::getActiveFlag()
{
   return m_active;
}

/* setFlagForFixedLocationInWindow: set flag for fixed location in window */
void Julius_Logger::setFlagForFixedLocationInWindow(bool flag)
{
   m_flagForFixedLocationInWindow = flag;
}

/* getFlagForFixedLocationInWindow: get flag for fixed location in window */
bool Julius_Logger::getFlagForFixedLocationInWindow()
{
   return m_flagForFixedLocationInWindow;
}

/* update: update log view per step */
void Julius_Logger::update(double frame)
{
   int i;
   int currentFrame;
   int np, ni;
   TRELLIS_ATOM *tre, *tre2;
   float h1, h2, x1, x2;

   if (m_active == false || m_numWord == 0)
      return;

   m_adInFrameStep += frame;
   if (m_adInFrameStep >= JULIUSLOGGER_ADINMAXVOLUMEUPDATEFRAME) {
      m_adInFrameStep = 0.0;
      if (m_currentMaxAdIn >= JULIUSLOGGER_ADINOVERFLOWTHRES)
         m_maxAdIn = 1.0f;
      else
         m_maxAdIn = (float) m_currentMaxAdIn / 32767.0f;
      m_currentMaxAdIn = 0;
   }

   if (m_lastTrellis) {
      currentFrame = m_lastTrellis->endtime;
      np = 0;
      ni = 0;
      for (tre = m_lastTrellis; tre; tre = tre->next) {
         if (tre->endtime != currentFrame) break;
         for (tre2 = tre; tre2 && tre2->wid != WORD_INVALID; tre2 = tre2->last_tre) {
            if (tre2->last_tre->wid == WORD_INVALID)
               h1 = 0.0f;
            else
               h1 = 1.0f - tre2->last_tre->wid / (float) m_numWord;
            h2 = 1.0f - tre->wid / (float) m_numWord;
            x1 = 1.0f - (currentFrame - tre2->last_tre->endtime) / JULIUSLOGGER_FRAMESINBAR;
            x2 = 1.0f - (currentFrame - tre2->endtime) / JULIUSLOGGER_FRAMESINBAR;
            m_pos[np * 3] = JULIUSLOGGER_BARMARGIN * m_marginScale + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2 * m_marginScale) * x1;
            m_pos[np * 3 + 1] = JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARHEIGHT - JULIUSLOGGER_BARMARGIN * 2) * h1;
            m_pos[np * 3 + 2] = JULIUSLOGGER_BARINDICATORZOFFSET;
            np++;
            m_pos[np * 3] = JULIUSLOGGER_BARMARGIN * m_marginScale + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2 * m_marginScale) * x2;
            m_pos[np * 3 + 1] = JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARHEIGHT - JULIUSLOGGER_BARMARGIN * 2) * h2;
            m_pos[np * 3 + 2] = JULIUSLOGGER_BARINDICATORZOFFSET;
            np++;
            m_index[ni++] = np - 2;
            m_index[ni++] = np - 1;
            if (np >= JULIUSLOGGER_MAXARCS || ni >= JULIUSLOGGER_MAXARCS) {
               tre = NULL;
               break;
            }
         }
         if (tre == NULL) break;
      }
      m_numPos = np;
      m_numIndex = ni;
   } else if (m_decayFrame > 0.0) {
      for (i = 0; i < m_numPos; i++)
         m_pos[i * 3 + 1] *= JULIUSLOGGER_DECAYDECREASECOEF;
   }
   if (m_decayFrame > 0.0) {
      m_decayFrame -= frame;
      if (m_decayFrame < 0.0) m_decayFrame = 0.0;
   }

}

/* render: render log view */
void Julius_Logger::render()
{
   GLfloat squareVers[] = { 0.0f, 0.0f, -0.01f, 0.0f, JULIUSLOGGER_BARHEIGHT, -0.01f, JULIUSLOGGER_BARWIDTH, 0.0f, -0.01f, JULIUSLOGGER_BARWIDTH, JULIUSLOGGER_BARHEIGHT, -0.01f };
   GLfloat edgeVers[] = { 0.0f, 0.0f, -0.01f, 0.0f, JULIUSLOGGER_BARHEIGHT, -0.01f, JULIUSLOGGER_BARWIDTH, JULIUSLOGGER_BARHEIGHT, -0.01f, JULIUSLOGGER_BARWIDTH, 0.0f, -0.01f };
   GLfloat barVers[] = { JULIUSLOGGER_BARMARGIN, JULIUSLOGGER_BARMARGIN, 0.0f, JULIUSLOGGER_BARMARGIN, JULIUSLOGGER_BARHEIGHT - JULIUSLOGGER_BARMARGIN, 0.0f, JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2) * m_maxAdIn, JULIUSLOGGER_BARMARGIN, 0.0f, JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2) * m_maxAdIn, JULIUSLOGGER_BARHEIGHT - JULIUSLOGGER_BARMARGIN, 0.0f };
   GLfloat triggerLevelVers[] = { JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2) * m_levelThreshold, JULIUSLOGGER_BARHEIGHT - JULIUSLOGGER_BARMARGIN, 0.01f, JULIUSLOGGER_BARMARGIN + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2) * m_levelThreshold, JULIUSLOGGER_BARMARGIN, 0.01f };

   if (m_active == false || m_numWord == 0)
      return;

   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_CULL_FACE);
   if (m_flagForFixedLocationInWindow) {
      /* save projection matrix */
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      /* set orthographic projection fitted to the bar */
      glLoadIdentity();
      MMDAgent_setOrtho(0, JULIUSLOGGER_BARWIDTH, 0, JULIUSLOGGER_BARHEIGHT * JULIUSLOGGER_BARFACTORFORFIXEDLOCATION, -1, 1);
      /* save modelview matrix*/
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      /* reset modelview matrix */
      glLoadIdentity();
      /* update margin scale */
      GLint viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);
      m_marginScale = (float)(JULIUSLOGGER_BARWIDTH * viewport[3]) / (float)(JULIUSLOGGER_BARHEIGHT * viewport[2] * JULIUSLOGGER_BARFACTORFORFIXEDLOCATION);
      barVers[0] = barVers[3] = JULIUSLOGGER_BARMARGIN * m_marginScale;
      barVers[6] = barVers[9] = JULIUSLOGGER_BARMARGIN * m_marginScale + (JULIUSLOGGER_BARWIDTH - JULIUSLOGGER_BARMARGIN * 2 * m_marginScale) * m_maxAdIn;
   } else {
      glPushMatrix();
      glTranslatef(-4.0f, 12.0f, 3.0f);
      m_marginScale = 1.0f;
   }

   /* square */
   glNormal3f(0.0, 0.0, 1.0);
   glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, squareVers);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glDisableClientState(GL_VERTEX_ARRAY);

   /* edge */
   if (m_recognizing) {
      glColor4f(1.0f, 0.8f, 0.8f, 1.0f);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, edgeVers);
      glDrawArrays(GL_LINE_LOOP, 0, 4);
      glDisableClientState(GL_VERTEX_ARRAY);
   }

   /* bar */
   if(m_maxAdIn == 1.0f)
      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
   else
      glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, barVers);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   glDisableClientState(GL_VERTEX_ARRAY);

   /* trigger level */
   glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, triggerLevelVers);
   glDrawArrays(GL_LINES, 0, 2);
   glDisableClientState(GL_VERTEX_ARRAY);

   /* trellis */
   if (m_lastTrellis != NULL || m_decayFrame > 0.0f) {
      glColor4f(0.8f, 0.4f, 0.0f, 1.0f);
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, m_pos);
      glDrawElements(GL_LINES, m_numIndex, GL_UNSIGNED_SHORT, m_index);
      glDisableClientState(GL_VERTEX_ARRAY);
   }

   glPopMatrix();
   if (m_flagForFixedLocationInWindow) {
      /* restore projection matrix */
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      /* switch to model view mode */
      glMatrixMode(GL_MODELVIEW);
   }
   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
}

