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

#include <stdarg.h>
#include "MMDAgent.h"

/* LogText::initialize: initialize logger */
void LogText::initialize()
{
   m_font = NULL;
   m_textWidth = 0;
   m_textHeight = 0;
   m_textX = 0.0;
   m_textY = 0.0;
   m_textZ = 0.0;
   m_textScale = 0.0;

   m_textList = NULL;
   m_drawElements = NULL;

   m_textIndex = 0;
   m_viewIndex = 0;
}

/* LogText::clear: free logger */
void LogText::clear()
{
   int i;

   if (m_textList) {
      for (i = 0; i < LOGTEXT_MAXNLINES; i++)
         free(m_textList[i]);
      free(m_textList);
   }
   if (m_drawElements) {
      for(i = 0; i < LOGTEXT_MAXNLINES; i++) {
         if(m_drawElements[i].vertices) free(m_drawElements[i].vertices);
         if(m_drawElements[i].texcoords) free(m_drawElements[i].texcoords);
         if(m_drawElements[i].indices) free(m_drawElements[i].indices);
      }
      free(m_drawElements);
   }

   initialize();
}

/* LogText::LogText: constructor */
LogText::LogText()
{
   initialize();
}

/* LogText::~LogText: destructor */
LogText::~LogText()
{
   clear();
}

/* LogText::setup: initialize and setup logger with args */
void LogText::setup(FTGLTextureFont *font, const int *size, const float *position, float scale)
{
   int i;

   if (size[0] <= 0 || size[1] <= 0 || scale <= 0.0) return;

   clear();

   m_font = font;

   m_textWidth = size[0];
   m_textHeight = size[1];
   m_textX = position[0];
   m_textY = position[1];
   m_textZ = position[2];
   m_textScale = scale;

   m_textList = (char **) malloc(sizeof(char *) * LOGTEXT_MAXNLINES);
   for (i = 0; i < LOGTEXT_MAXNLINES; i++) {
      m_textList[i] = (char *) malloc(sizeof(char) * (m_textWidth + 1));
      strcpy(m_textList[i], "");
   }

   m_drawElements = (FTGLTextDrawElements *) malloc(sizeof(FTGLTextDrawElements) * LOGTEXT_MAXNLINES);
   for(i = 0; i < LOGTEXT_MAXNLINES; i++)
      memset(&m_drawElements[i], 0, sizeof(FTGLTextDrawElements));
}

/* LogText::log: store log text */
void LogText::log(const char *format, ...)
{
   char *p, *save, *c;
   char buff[MMDAGENT_MAXBUFLEN];
   va_list args;
   int i, len;
   unsigned char size;

   if (m_textList == NULL) return;

   va_start(args, format);
   vsprintf(buff, format, args);
   for (p = MMDAgent_strtok(buff, "\n", &save); p; p = MMDAgent_strtok(NULL, "\n", &save)) {
      len = MMDAgent_strlen(p);
      if (len > 0) {
         c = p;
         for (i = 0; i < len; i += size) {
            size = MMDAgent_getcharsize(c);
            if (size == 0)
               break;
            if (i + size >= m_textWidth) {
               *c = '\0';
               break;
            }
            c += size;
         }
      }
      strcpy(m_textList[m_textIndex], p);
      if (m_font->getTextDrawElements(m_textList[m_textIndex], &(m_drawElements[m_textIndex]), 0, 0.0f, 0.0f, 0.0f) == false) {
         m_drawElements[m_textIndex].textLen = 0; /* reset */
         m_drawElements[m_textIndex].numIndices = 0;
      }
      m_textIndex++;
      if (m_textIndex >= LOGTEXT_MAXNLINES)
         m_textIndex = 0;
      if(m_viewIndex != 0)
         scroll(1);
   }
   va_end(args);
}

/* LogText::scroll: scroll text area */
void LogText::scroll(int shift)
{
   if(LOGTEXT_MAXNLINES <= m_textHeight)
      return;

   m_viewIndex += shift;

   if(m_viewIndex < 0)
      m_viewIndex = 0;
   else if(m_viewIndex >= LOGTEXT_MAXNLINES - m_textHeight)
      m_viewIndex = LOGTEXT_MAXNLINES - m_textHeight;
}

/* LogText::render: render log text */
void LogText::render()
{
   int i, j, size;
   float w, h, rate;
   GLfloat vertices[12];
   GLubyte indices[] = { 0, 1, 2, 0, 2, 3 };

   if (m_textList == NULL) return;

   w = 0.5f * (float) (m_textWidth) * 0.85f + 1.0f;
   h = 1.0f * (float) (m_textHeight) * 0.85f + 1.0f;

   glPushMatrix();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glScalef(m_textScale, m_textScale, m_textScale);
   glNormal3f(0.0f, 1.0f, 0.0f);
   glEnableClientState(GL_VERTEX_ARRAY);

   /* background */
   glColor4f(LOGTEXT_BGCOLOR);
   vertices[0] = m_textX;
   vertices[1] = m_textY;
   vertices[2] = m_textZ;
   vertices[3] = m_textX + w;
   vertices[4] = m_textY;
   vertices[5] = m_textZ;
   vertices[6] = m_textX + w;
   vertices[7] = m_textY + h;
   vertices[8] = m_textZ;
   vertices[9] = m_textX;
   vertices[10] = m_textY + h;
   vertices[11] = m_textZ;
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

   /* scroll bar */
   if(m_textHeight < LOGTEXT_MAXNLINES) {
      glColor4f(LOGTEXT_COLOR);
      vertices[0] = m_textX + w;
      vertices[1] = m_textY;
      vertices[2] = m_textZ + 0.05f;
      vertices[3] = m_textX + w + LOGTEXT_SCROLLBARWIDTH;
      vertices[4] = m_textY;
      vertices[5] = m_textZ + 0.05f;
      vertices[6] = m_textX + w + LOGTEXT_SCROLLBARWIDTH;
      vertices[7] = m_textY + h;
      vertices[8] = m_textZ + 0.05f;
      vertices[9] = m_textX + w;
      vertices[10] = m_textY + h;
      vertices[11] = m_textZ + 0.05f;
      glVertexPointer(3, GL_FLOAT, 0, vertices);
      glDrawArrays(GL_LINE_LOOP, 0, 4);
      rate = (float)m_viewIndex / LOGTEXT_MAXNLINES;
      vertices[0] = m_textX + w;
      vertices[1] = m_textY + h * rate;
      vertices[2] = m_textZ + 0.05f;
      vertices[3] = m_textX + w + LOGTEXT_SCROLLBARWIDTH;
      vertices[4] = m_textY + h * rate;
      vertices[5] = m_textZ + 0.05f;
      rate = (float)(m_viewIndex + m_textHeight) / LOGTEXT_MAXNLINES;
      vertices[6] = m_textX + w + LOGTEXT_SCROLLBARWIDTH;
      vertices[7] = m_textY + h * rate;
      vertices[8] = m_textZ + 0.05f;
      vertices[9] = m_textX + w;
      vertices[10] = m_textY + h * rate;
      vertices[11] = m_textZ + 0.05f;
      glVertexPointer(3, GL_FLOAT, 0, vertices);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
   }

   /* text */
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   glClientActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, m_font->getTextureID());
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   glColor4f(LOGTEXT_COLOR);
   glTranslatef(m_textX + 0.5f, m_textY - 0.2f, m_textZ + 0.05f);
   size = LOGTEXT_MAXNLINES < m_textHeight ? LOGTEXT_MAXNLINES : m_textHeight;
   for(i = 0, j = m_textIndex - 1 - m_viewIndex; i < size; i++) {
      if(j < 0)
         j += LOGTEXT_MAXNLINES;
      glTranslatef(0.0f, 0.85f, 0.0f);
      if (m_drawElements[j].numIndices > 0) {
         glPushMatrix();
         glScalef(0.9f, 0.9f, 0.9f);
         glVertexPointer(3, GL_FLOAT, 0, m_drawElements[j].vertices);
         glTexCoordPointer(2, GL_FLOAT, 0, m_drawElements[j].texcoords);
         glDrawElements(GL_TRIANGLES, m_drawElements[j].numIndices, GL_UNSIGNED_SHORT, (const GLvoid *) m_drawElements[j].indices);
         glPopMatrix();
      }
      j--;
   }
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisable(GL_TEXTURE_2D);

   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
}
