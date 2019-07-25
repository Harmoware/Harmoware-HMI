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

/* definitions */

/* size, location, linespace, margins */
#define MENU_WIDTH                6     /* menu width */
#define MENU_HEIGHT_RATIO_DEFAULT 0.5f  /* default height of the menu, in ratio of screen height */
#define MENU_SCALEMIN             0.1f  /* minimum scale */
#define MENU_SCALEMAX             1.0f  /* maximum scale */
#define MENU_MARGIN_X             0.1f  /* X margin */
#define MENU_MARGIN_Y             0.1f  /* Y margin */
#define MENU_PADDING_X            0.2f  /* X padding space */
#define MENU_PADDING_Y            0.3f  /* Y padding space */
#define MENU_LINESPACE            0.2f  /* extra line space between items */

/* animation parameters */
#define MENU_DURATION_SHOWHIDE   6.0f  /* animation duration for show/hide */
#define MENU_DURATION_EXEC       7.0f  /* animation duration for item execution */
#define MENU_DURATION_FORWARD    7.0f  /* animation duration for moving forward */
#define MENU_DURATION_BACKWARD   7.0f  /* animation duration for moving backward */
#define MENU_DURATION_JUMP       7.0f  /* animation duration for jump */
#define MENU_DURATION_REGIST     6.0f  /* animation duration for regist */
#define MENU_ROTATION_AXIS       0.0f, 1.0f, -0.3f   /* rotation axis for moving */

/* colors */
#define MENU_COLOR_TITLE_CONTENT    0.1f, 0.7f, 0.5f, 0.7f  /* title background color of content stem */
#define MENU_COLOR_TITLE_SYSTEM     0.0f, 0.5f, 0.8f, 0.7f  /* title background color of system stem */
#define MENU_COLOR_TITLE_DEVELOP    0.0f, 0.2f, 0.9f, 0.7f  /* title background color of develop stem */
#define MENU_COLOR_ITEM_EXEC        0.9f, 0.0f, 0.1f, 0.7f  /* background color of execiting item */
#define MENU_COLOR_ITEM_CURSOR      0.8f, 0.3f, 0.0f, 0.7f  /* background color of item on cursor */
#define MENU_COLOR_STATE_NONE       0.0f, 0.0f, 0.6f, 0.6f  /* background color of non-configured item */
#define MENU_COLOR_STATE_NORMAL     0.0f, 0.0f, 0.7f, 0.6f  /* background color of item in normal state */
#define MENU_COLOR_STATE_DISABLED   0.0f, 0.0f, 0.0f, 0.6f  /* background color of item in disabled state */
#define MENU_COLOR_STATE_PRESSED    0.7f, 0.4f, 0.0f, 0.8f  /* background color of item in pressed state */
#define MENU_COLOR_STATE_UNKNOWN    1.0f, 0.0f, 0.0f, 0.8f  /* background color of item in unknown state */
#define MENU_COLOR_TEXT             0.8f, 0.8f, 0.8f, 1.0f  /* color of text */

/* Menu::initialize: initialize menu */
void Menu::initialize()
{
   GLint viewport[4];
   int i;

   for (i = 0; i < MENUMAXNUM; i++)
      initializeStem(i);

   m_mmdagent = NULL;
   m_font = NULL;

   m_currentCountId = 0;
   m_stemMaxNum = 0;

   m_needsUpdate = false;
   m_orderNum = 0;
   m_currentPos = 0;
   m_currentCursor = -1;
   m_showing = false;

   m_orientation = MENU_ORIENTATION;
   m_cWidth = (float)MENU_WIDTH + MENU_PADDING_X;
   m_cHeight = (1.0f + MENU_LINESPACE) * (MENUMAXITEM + 1) + MENU_PADDING_Y;
   m_size = MENU_HEIGHT_RATIO_DEFAULT;

   glGetIntegerv(GL_VIEWPORT, viewport);
   m_viewWidth = viewport[2];
   m_viewHeight = viewport[3];
   updateMenuPosition();

   m_cursorShow = false;
   m_showHideAnimationFrameLeft = MENU_DURATION_SHOWHIDE;
   m_execItemAnimationFrameLeft = 0.0f;
   m_forwardAnimationFrameLeft = 0.0f;
   m_backwardAnimationFrameLeft = 0.0f;
   m_jumpAnimationFrameLeft = 0.0f;
   m_isRegisting = false;
   m_forwardFrameForced = false;
   m_backwardFrameForced = false;
}

/* Menu::clear: free menu */
void Menu::clear()
{
   int i;

   for (i = 0; i < MENUMAXNUM; i++)
      clearStem(i);

   initialize();
}

/* Menu::initializeStem: initialize a stem */
void Menu::initializeStem(int id)
{
   int i;

   m_stem[id].active = false;
   m_stem[id].priority = MENUPRIORITY_CONTENT;
   m_stem[id].countId = 0;
   m_stem[id].name = NULL;
   for (i = 0; i < MENUMAXITEM; i++) {
      m_stem[id].itemName[i] = NULL;
      m_stem[id].messageType[i] = NULL;
      m_stem[id].messageArg[i] = NULL;
      m_stem[id].status[i] = MENUITEM_STATUS_NONE;
   }
   m_stem[id].func = NULL;
   m_stem[id].func = NULL;
   m_stem[id].data = NULL;
   memset(&(m_stem[id].elem), 0, sizeof(FTGLTextDrawElements));
   m_stem[id].needsUpdate = false;
}

/* Menu::clearStem: clear a stem */
void Menu::clearStem(int id)
{
   int i;

   if (m_stem[id].name)
      free(m_stem[id].name);
   for (i = 0; i < MENUMAXITEM; i++) {
      if (m_stem[id].itemName[i])
         free(m_stem[id].itemName[i]);
      if (m_stem[id].messageType[i])
         free(m_stem[id].messageType[i]);
      if (m_stem[id].messageArg[i])
         free(m_stem[id].messageArg[i]);
   }
   if (m_stem[id].elem.vertices) free(m_stem[id].elem.vertices);
   if (m_stem[id].elem.texcoords) free(m_stem[id].elem.texcoords);
   if (m_stem[id].elem.indices) free(m_stem[id].elem.indices);
}

/* Menu::updatePosition: update menu position */
void Menu::updateMenuPosition()
{
   float scale, rate;

   /* compute relative height from aspect */
   rate = sqrtf((float)m_viewWidth / (float)m_viewHeight) * 1.333f * m_size;
   if (rate < 0.5f)
      rate = 0.5f;
   if (rate > 1.0f)
      rate = 1.0f;
   scale = rate * m_viewHeight / m_cHeight;
   m_width = (float)m_viewWidth / scale;
   m_height = (float)m_viewHeight / scale;

   /* compute position and range */
   if (m_orientation == MENU_ORIENTATION_TOP_LEFT || m_orientation == MENU_ORIENTATION_BOTTOM_LEFT) {
      /* aligned to left */
      m_posX = MENU_MARGIN_X;
      m_rx1 = 0.0f;
      m_rx2 = m_cWidth / m_width;
   } else {
      /* aligned to right */
      m_posX = m_width - MENU_MARGIN_X;
      m_rx1 = 1.0f - m_cWidth / m_width;
      m_rx2 = 1.0f;
   }
   if (m_orientation == MENU_ORIENTATION_TOP_LEFT || m_orientation == MENU_ORIENTATION_TOP_RIGHT) {
      /* aligned to top */
      m_posY = m_height - MENU_MARGIN_Y;
      m_ry1 = 1.0f - m_cHeight / m_height;
      m_ry2 = 1.0f;
   } else {
      /* aligned to bottom */
      m_posY = MENU_MARGIN_Y;
      m_ry1 = 0.0f;
      m_ry2 = m_cHeight / m_height;
   }
}

/* Menu::updateStem: update stem for rendering */
void Menu::updateStem(int id)
{
   int i;
   float y, h;
   float x1, x2;
   float width;

   if (m_font == NULL)
      return;

   if (m_stem[id].active == false)
      return;

   m_stem[id].elem.textLen = 0;
   m_stem[id].elem.numIndices = 0;

   h = 1.0f + MENU_LINESPACE;
   width = m_cWidth;

   if (m_orientation == MENU_ORIENTATION_TOP_LEFT || m_orientation == MENU_ORIENTATION_BOTTOM_LEFT) {
      /* aligned to left */
      x1 = 0.0f;
      x2 = width;
   } else {
      /* aligned to right */
      x1 = -width;
      x2 = 0.0f;
   }
   if (m_orientation == MENU_ORIENTATION_TOP_LEFT || m_orientation == MENU_ORIENTATION_TOP_RIGHT) {
      /* aligned to top */
      y = 0.0f;
   } else {
      /* aligned to bottom */
      y = h * (float)(MENUMAXITEM + 1);
   }

   y -= h;
   if (m_font->getTextDrawElements(m_stem[id].name, &(m_stem[id].elem), m_stem[id].elem.textLen, x1 + MENU_PADDING_X, y + MENU_PADDING_Y, 0.0f) == false) {
      m_stem[id].elem.textLen = 0; /* reset */
      m_stem[id].elem.numIndices = 0;
      return;
   }
   m_stem[id].vertices[0][0] = x1;
   m_stem[id].vertices[0][1] = y + h;
   m_stem[id].vertices[0][2] = -0.1f;
   m_stem[id].vertices[0][3] = x1;
   m_stem[id].vertices[0][4] = y;
   m_stem[id].vertices[0][5] = -0.1f;
   m_stem[id].vertices[0][6] = x2;
   m_stem[id].vertices[0][7] = y;
   m_stem[id].vertices[0][8] = -0.1f;
   m_stem[id].vertices[0][9] = x2;
   m_stem[id].vertices[0][10] = y + h;
   m_stem[id].vertices[0][11] = -0.1f;

   y -= h;
   for (i = 0; i < MENUMAXITEM; i++) {
      if (m_stem[id].itemName[i] != NULL) {
         if (m_font->getTextDrawElements(m_stem[id].itemName[i], &(m_stem[id].elem), m_stem[id].elem.textLen, x1 + MENU_PADDING_X, y + MENU_PADDING_Y, 0.0f) == false) {
            m_stem[id].elem.textLen = 0; /* reset */
            m_stem[id].elem.numIndices = 0;
            free(m_stem[id].itemName[i]);
            m_stem[id].itemName[i] = NULL;
            return;
         }
      }
      m_stem[id].vertices[i + 1][0] = x1;
      m_stem[id].vertices[i + 1][1] = y + h;
      m_stem[id].vertices[i + 1][2] = -0.1f;
      m_stem[id].vertices[i + 1][3] = x1;
      m_stem[id].vertices[i + 1][4] = y;
      m_stem[id].vertices[i + 1][5] = -0.1f;
      m_stem[id].vertices[i + 1][6] = x2;
      m_stem[id].vertices[i + 1][7] = y;
      m_stem[id].vertices[i + 1][8] = -0.1f;
      m_stem[id].vertices[i + 1][9] = x2;
      m_stem[id].vertices[i + 1][10] = y + h;
      m_stem[id].vertices[i + 1][11] = -0.1f;
      y -= h;
   }
}

/* Menu::sortStem: sort stem */
void Menu::sortStem()
{
   int i, k;
   bool swapped;

   k = 0;
   for (i = 0; i < m_stemMaxNum; i++) {
      if (m_stem[i].active == false)
         continue;
      m_order[k] = i;
      k++;
   }
   m_orderNum = k;

   /* sort by priority and countId, store order in m_order[] */
   do {
      swapped = false;
      for (i = 0; i < m_orderNum - 1; i++) {
         if (m_stem[m_order[i]].priority > m_stem[m_order[i + 1]].priority
               || (m_stem[m_order[i]].priority == m_stem[m_order[i + 1]].priority && m_stem[m_order[i]].countId < m_stem[m_order[i + 1]].countId)) {
            k = m_order[i];
            m_order[i] = m_order[i + 1];
            m_order[i + 1] = k;
            swapped = true;
         }
      }
   } while (swapped == true);

}

/* Menu::renderBegin: render begin */
void Menu::renderBegin()
{
   GLint viewport[4];

   glGetIntegerv(GL_VIEWPORT, viewport);
   if (m_viewWidth != viewport[2] || m_viewHeight != viewport[3]) {
      m_viewWidth = viewport[2];
      m_viewHeight = viewport[3];
      updateMenuPosition();
   }

   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   MMDAgent_setOrtho(0, m_width, 0, m_height, -MENU_WIDTH, MENU_WIDTH);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glDepthFunc(GL_LEQUAL);
   glEnableClientState(GL_VERTEX_ARRAY);
   glTranslatef(m_posX, m_posY, 0.0f);
}

/* Menu::renderStem: render a stem */
void Menu::renderStem(int id)
{
   int i;
   GLushort bgIndices[6] = {0, 1, 2, 0, 2, 3};
   float r;

   switch (m_stem[id].priority) {
   case MENUPRIORITY_CONTENT:
      glColor4f(MENU_COLOR_TITLE_CONTENT);
      break;
   case MENUPRIORITY_SYSTEM:
      glColor4f(MENU_COLOR_TITLE_SYSTEM);
      break;
   case MENUPRIORITY_DEVELOP:
      glColor4f(MENU_COLOR_TITLE_DEVELOP);
      break;
   }
   glVertexPointer(3, GL_FLOAT, 0, m_stem[id].vertices[0]);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)bgIndices);

   for (i = 0; i < MENUMAXITEM; i++) {
      if (m_execItemAnimationFrameLeft > 0.0f &&  id == m_execPos && i == m_execItemId) {
         glPushMatrix();
         r = m_execItemAnimationFrameLeft / MENU_DURATION_EXEC;
         r = - 2.0f * r * (1.0f - r);
         glTranslatef(r, 0.0f, 0.0f);
         glColor4f(MENU_COLOR_ITEM_EXEC);
      } else if (m_cursorShow == true && id == m_order[m_currentPos] && i == m_currentCursor) {
         glColor4f(MENU_COLOR_ITEM_CURSOR);
      } else {
         switch (m_stem[id].status[i]) {
         case MENUITEM_STATUS_NONE:
            glColor4f(MENU_COLOR_STATE_NONE);
            break;
         case MENUITEM_STATUS_NORMAL:
            glColor4f(MENU_COLOR_STATE_NORMAL);
            break;
         case MENUITEM_STATUS_DISABLED:
            glColor4f(MENU_COLOR_STATE_DISABLED);
            break;
         case MENUITEM_STATUS_PRESSED:
            glColor4f(MENU_COLOR_STATE_PRESSED);
            break;
         default:
            glColor4f(MENU_COLOR_STATE_UNKNOWN);
         }
      }
      glVertexPointer(3, GL_FLOAT, 0, m_stem[id].vertices[i + 1]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const GLvoid *)bgIndices);
      if (m_execItemAnimationFrameLeft > 0.0f &&  id == m_execPos && i == m_execItemId)
         glPopMatrix();
   }
   if (m_stem[id].elem.numIndices > 0) {
      glEnable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);
      glClientActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_font->getTextureID());
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glColor4f(MENU_COLOR_TEXT);
      glVertexPointer(3, GL_FLOAT, 0, m_stem[id].elem.vertices);
      glTexCoordPointer(2, GL_FLOAT, 0, m_stem[id].elem.texcoords);
      glDrawElements(GL_TRIANGLES, m_stem[id].elem.numIndices, GL_UNSIGNED_SHORT, (const GLvoid *)m_stem[id].elem.indices);
   }

   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisable(GL_TEXTURE_2D);
}

/* Menu::renderEnd: render end */
void Menu::renderEnd()
{
   glDepthFunc(GL_LESS);
   glDisableClientState(GL_VERTEX_ARRAY);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
}

/* Menu: constructor */
Menu::Menu()
{
   initialize();
}

/* Menu::~Menu: destructor */
Menu::~Menu()
{
   clear();
}

/* Menu::setup: initialize and setup menu */
void Menu::setup(MMDAgent *mmdagent, FTGLTextureFont *font)
{
   clear();
   m_mmdagent = mmdagent;
   m_font = font;
}

/* Menu::add: add a new stem and return its id */
int Menu::add(const char *name, int priority, void(*func)(int id, int row, void *data), void *data)
{
   int i;

   for (i = 0; i < MENUMAXNUM; i++) {
      if (m_stem[i].active == false) {
         /* found inactive stem, use it */
         /* make sure it's clean */
         clearStem(i);
         /* set initial values */
         m_stem[i].name = MMDAgent_strdup(name);
         m_stem[i].func = func;
         m_stem[i].data = data;
         m_stem[i].priority = priority;
         /* set this as active */
         m_stem[i].active = true;
         /* update stem max num */
         if (m_stemMaxNum < i + 1)
            m_stemMaxNum = i + 1;
         /* update count id */
         m_stem[i].countId = m_currentCountId++;
         /* mark as needs update, this stem and whole */
         m_stem[i].needsUpdate = true;
         m_needsUpdate = true;
         return i;
      }
   }

   return -1;
}

/* Menu::find: find stem by name, and return the id */
int Menu::find(const char *name)
{
   int i;

   for (i = 0; i < m_stemMaxNum; i++) {
      if (m_stem[i].active == false)
         continue;
      if (MMDAgent_strequal(m_stem[i].name, name))
         return i;
   }

   return -1;
}

/* Menu::setName: set name of the stem */
bool Menu::setName(int id, const char *name)
{
   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return false;

   if (m_stem[id].name)
      free(m_stem[id].name);
   m_stem[id].name = MMDAgent_strdup(name);

   m_stem[id].needsUpdate = true;

   return true;
}

/* Menu::setItem: set an item to the stem */
bool Menu::setItem(int id, int row, const char *label, const char *messageType, const char *messageArg)
{
   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return false;

   if (row < 0 || row >= MENUMAXITEM)
      return false;

   if (m_stem[id].itemName[row])
      free(m_stem[id].itemName[row]);
   m_stem[id].itemName[row] = MMDAgent_strdup(label);
   if (m_stem[id].messageType[row])
      free(m_stem[id].messageType[row]);
   m_stem[id].messageType[row] = MMDAgent_strdup(messageType);
   if (m_stem[id].messageArg[row])
      free(m_stem[id].messageArg[row]);
   m_stem[id].messageArg[row] = MMDAgent_strdup(messageArg);
   if (m_stem[id].status[row] == MENUITEM_STATUS_NONE)
      m_stem[id].status[row] = MENUITEM_STATUS_NORMAL;
   m_stem[id].needsUpdate = true;

   return true;
}

/* Menu::findItem: find item of the stem by label, and return the row */
int Menu::findItem(int id, const char *label)
{
   int i;

   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return -1;

   for (i = 0; i < MENUMAXITEM; i++) {
      if (m_stem[id].itemName[i] != NULL && MMDAgent_strequal(m_stem[id].itemName[i], label))
         return i;
   }

   return -1;
}

/* Menu::setItemLabel: set item label in the stem */
bool Menu::setItemLabel(int id, int row, const char *label)
{
   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return false;

   if (row < 0 || row >= MENUMAXITEM)
      return false;

   if (m_stem[id].status[row] != MENUITEM_STATUS_NONE) {
      if (m_stem[id].itemName[row])
         free(m_stem[id].itemName[row]);
      m_stem[id].itemName[row] = MMDAgent_strdup(label);
   }

   m_stem[id].needsUpdate = true;

   return true;
}

/* Menu::setItemStatus: set item status of the stem */
bool Menu::setItemStatus(int id, int row, int status)
{
   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return false;

   if (row < 0 || row >= MENUMAXITEM) {
      return false;
   }

   m_stem[id].status[row] = status;

   return true;
}

/* Menu::removeItem: remove item of the stem */
bool Menu::removeItem(int id, int row)
{
   if (id < 0 || id >= m_stemMaxNum || m_stem[id].active == false)
      return false;

   if (row < 0 || row >= MENUMAXITEM)
      return false;

   if (id == m_order[m_currentPos] && row == m_currentCursor)
      m_currentCursor = -1;

   if (m_stem[id].itemName[row])
      free(m_stem[id].itemName[row]);
   m_stem[id].itemName[row] = NULL;
   if (m_stem[id].messageType[row])
      free(m_stem[id].messageType[row]);
   m_stem[id].messageType[row] = NULL;
   if (m_stem[id].messageArg[row])
      free(m_stem[id].messageArg[row]);
   m_stem[id].messageArg[row] = NULL;
   m_stem[id].status[row] = MENUITEM_STATUS_NONE;

   m_stem[id].needsUpdate = true;

   return true;
}

/* Menu::remove: remove the stem */
bool Menu::remove(int id)
{
   if (id < 0 || id >= MENUMAXNUM)
      return false;

   if (id == m_order[m_currentPos])
      m_currentCursor = -1;

   clearStem(id);
   initializeStem(id);

   m_stem[id].active = false;

   m_needsUpdate = true;

   return true;
}

/* Menu::isShowing: return true when showing */
bool Menu::isShowing()
{
   return m_showing;
}

/* Menu::getSize: return menu size */
float Menu::getSize()
{
   return m_size;
}

/* Menu::setSize: set menu size */
void Menu::setSize(float size)
{
   m_size = size;
   if (m_size < MENU_SCALEMIN)
      m_size = MENU_SCALEMIN;
   if (m_size > MENU_SCALEMAX)
      m_size = MENU_SCALEMAX;
   updateMenuPosition();
}

/* Menu::setOrientation: set orientation */
void Menu::setOrientation(int orientation)
{
   int i;

   m_orientation = orientation;
   updateMenuPosition();
   for (i = 0; i < m_stemMaxNum; i++) {
      if (m_stem[i].active == false)
         continue;
      m_stem[i].needsUpdate = true;
   }
}

/* Menu::show: turn on this menu*/
void Menu::show()
{
   if (m_showing == false)
      m_showing = true;
}

/* Menu::hide: rurn off this menu */
void Menu::hide()
{
   if (m_showing == true)
      m_showing = false;
}

/* Menu::forward: move the menus forward */
void Menu::forward()
{
   if (m_currentPos > m_orderNum - 1) {
      m_currentPos = m_orderNum - 1;
   } else if (m_currentPos == m_orderNum - 1) {
      m_forwardAnimationFrameLeft = 0.0f;
      m_backwardAnimationFrameLeft = 0.0f;
      m_jumpAnimationFrameLeft = 0.0f;
      m_forwardRegistAnimationFrameLeft = MENU_DURATION_REGIST;
      m_backwardRegistAnimationFrameLeft = 0.0f;
      m_isRegisting = true;
   } else {
      m_forwardAnimationFrameLeft = MENU_DURATION_FORWARD;
      m_backwardAnimationFrameLeft = 0.0f;
      m_jumpAnimationFrameLeft = 0.0f;
      m_forwardRegistAnimationFrameLeft = 0.0f;
      m_backwardRegistAnimationFrameLeft = 0.0f;
      m_currentPos++;
      m_isRegisting = false;
   }
}

/* Menu::backward: move the menus backward */
void Menu::backward()
{
   if (m_currentPos < 0) {
      m_currentPos = 0;
   } else if (m_currentPos == 0) {
      m_forwardAnimationFrameLeft = 0.0f;
      m_backwardAnimationFrameLeft = 0.0f;
      m_jumpAnimationFrameLeft = 0.0f;
      m_forwardRegistAnimationFrameLeft = 0.0f;
      m_backwardRegistAnimationFrameLeft = MENU_DURATION_REGIST;;
      m_isRegisting = true;
   } else {
      m_forwardAnimationFrameLeft = 0.0f;
      m_backwardAnimationFrameLeft = MENU_DURATION_BACKWARD;
      m_jumpAnimationFrameLeft = 0.0f;
      m_forwardRegistAnimationFrameLeft = 0.0f;
      m_backwardRegistAnimationFrameLeft = 0.0f;
      m_currentPos--;
      m_isRegisting = false;
   }
}

/* Menu::jump: bring the specified stem to front */
void Menu::jump(int id)
{
   if (m_currentPos < 0) {
      m_currentPos = 0;
   } else if (m_currentPos > m_orderNum - 1) {
      m_currentPos = m_orderNum - 1;
   } else {
      m_forwardAnimationFrameLeft = 0.0f;
      m_backwardAnimationFrameLeft = 0.0f;
      m_jumpAnimationFrameLeft = MENU_DURATION_JUMP;
      m_forwardRegistAnimationFrameLeft = 0.0f;
      m_backwardRegistAnimationFrameLeft = 0.0f;
      m_currentPos = id;
   }
}

/* Menu::moveCursorUp: move cursor up */
void Menu::moveCursorUp()
{
   int id;
   int n;

   id = m_order[m_currentPos];
   n = m_currentCursor - 1;
   /* move to the first valid items */
   while (n >= 0) {
      if (m_stem[id].status[n] != MENUITEM_STATUS_NONE) {
         m_currentCursor = n;
         break;
      }
      n--;
   }

   m_cursorShow = true;
}

/* Menu::moveCursorDown: move cursor down */
void Menu::moveCursorDown()
{
   int id;
   int n;

   id = m_order[m_currentPos];
   n = m_currentCursor + 1;
   /* move to the first valid items */
   while (n < MENUMAXITEM) {
      if (m_stem[id].status[n] != MENUITEM_STATUS_NONE) {
         m_currentCursor = n;
         break;
      }
      n++;
   }

   m_cursorShow = true;
}

/* Menu::execItem: execute the item at the cursor */
void Menu::execCurrentItem()
{
   if (m_currentCursor == -1)
      return;

   execItem(m_currentCursor);
}

/* Menu::execItem: execute the item of the stem */
void Menu::execItem(int choice)
{
   int id;

   id = m_order[m_currentPos];

   if (choice < 0 || choice > MENUMAXITEM - 1)
      return;

   if (m_stem[id].status[choice] == MENUITEM_STATUS_NONE || m_stem[id].status[choice] == MENUITEM_STATUS_DISABLED)
      return;

   /* set animation status to show executed animation */
   m_execItemAnimationFrameLeft = MENU_DURATION_EXEC;
   m_execPos = id;
   m_execItemId = choice;

   if (m_stem[id].messageType[choice] || m_stem[id].messageArg[choice])
      m_mmdagent->sendMessage(m_stem[id].messageType[choice], "%s", m_stem[id].messageArg[choice]);

   if (m_stem[id].func)
      m_stem[id].func(id, choice, m_stem[id].data);
}

/* Menu::execByTap: execute the item of the stem at tapped point */
int Menu::execByTap(int x, int y, int screenWidth, int screenHeight)
{
   float rx, ry;
   int n;

   rx = x / (float)screenWidth;
   ry = 1.0f - y / (float)screenHeight;

   if (rx < m_rx1 || rx > m_rx2 || ry < m_ry1 || ry > m_ry2)
      return -1;

   n = (int)((MENUMAXITEM + 1) * (m_ry2 - ry) / (m_ry2 - m_ry1));
   if (n > MENUMAXITEM)
      n = MENUMAXITEM;

   if (n == 0) {
      /* tapped menu title */
   } else {
      /* tapped menu item */
      execItem(n - 1);
   }

   return n - 1;
}

/* Menu::isPointed: return true when pointed */
bool Menu::isPointed(int x, int y, int screenWidth, int screenHeight)
{
   float rx, ry;

   rx = x / (float)screenWidth;
   ry = 1.0f - y / (float)screenHeight;

   if (rx < m_rx1 || rx > m_rx2 || ry < m_ry1 || ry > m_ry2)
      return false;

   return true;
}

/* Menu::forceForwardAnimationRate: force forward animation rate */
void Menu::forceForwardAnimationRate(float rate)
{
   if (rate < 0.0f) {
      /* release force mode */
      m_forwardFrameForced = false;
      return;
   }
   if (rate > 1.0f)
      rate = 1.0;
   if (m_isRegisting)
      m_forwardRegistAnimationFrameLeft = MENU_DURATION_REGIST * (1.0f - rate);
   else
      m_forwardAnimationFrameLeft = MENU_DURATION_FORWARD * (1.0f - rate);
   m_forwardFrameForced = true;
}

/* Menu::forceBackwardAnimationRate: force backward animation rate */
void Menu::forceBackwardAnimationRate(float rate)
{
   if (rate < 0.0f) {
      /* release force mode */
      m_backwardFrameForced = false;
      return;
   }
   if (rate > 1.0f)
      rate = 1.0;
   if (m_isRegisting)
      m_backwardRegistAnimationFrameLeft = MENU_DURATION_REGIST * (1.0f - rate);
   else
      m_backwardAnimationFrameLeft = MENU_DURATION_BACKWARD * (1.0f - rate);
   m_backwardFrameForced = true;
}

/* Menu::update: if needs update, sort by priority and update menu */
void Menu::update(double ellapsedFrame)
{
   int i;

   if (m_needsUpdate) {
      /* update whole */
      sortStem();
      m_needsUpdate = false;
   }
   /* re-construct vertex array for stems with needsUpdate flags */
   for (i = 0; i < m_stemMaxNum; i++) {
      if (m_stem[i].active == false)
         continue;
      if (m_stem[i].needsUpdate == true) {
         updateStem(i);
         m_stem[i].needsUpdate = false;
      }
   }
   /* decrement animation durations */
   if (m_showing) {
      m_showHideAnimationFrameLeft -= (float)ellapsedFrame;
      if (m_showHideAnimationFrameLeft < 0.0f)
         m_showHideAnimationFrameLeft = 0.0f;
   } else {
      m_showHideAnimationFrameLeft += (float)ellapsedFrame;
      if (m_showHideAnimationFrameLeft > MENU_DURATION_SHOWHIDE)
         m_showHideAnimationFrameLeft = MENU_DURATION_SHOWHIDE;
   }
   if (m_forwardFrameForced == false) {
      m_forwardAnimationFrameLeft -= (float)ellapsedFrame;
      if (m_forwardAnimationFrameLeft < 0.0f)
         m_forwardAnimationFrameLeft = 0.0f;
      m_forwardRegistAnimationFrameLeft -= (float)ellapsedFrame;
      if (m_forwardRegistAnimationFrameLeft < 0.0f)
         m_forwardRegistAnimationFrameLeft = 0.0f;
   }
   if (m_backwardFrameForced == false) {
      m_backwardAnimationFrameLeft -= (float)ellapsedFrame;
      if (m_backwardAnimationFrameLeft < 0.0f)
         m_backwardAnimationFrameLeft = 0.0f;
      m_backwardRegistAnimationFrameLeft -= (float)ellapsedFrame;
      if (m_backwardRegistAnimationFrameLeft < 0.0f)
         m_backwardRegistAnimationFrameLeft = 0.0f;
   }
   m_execItemAnimationFrameLeft -= (float)ellapsedFrame;
   if (m_execItemAnimationFrameLeft < 0.0f)
      m_execItemAnimationFrameLeft = 0.0f;
   m_jumpAnimationFrameLeft -= (float)ellapsedFrame;
   if (m_jumpAnimationFrameLeft < 0.0f)
      m_jumpAnimationFrameLeft = 0.0f;
}

/* Menu::render: render the menu structure */
void Menu::render()
{
   int id;
   float r;

   if (m_font == NULL)
      return;

   if (m_orderNum == 0)
      return;

   if (m_showing == false && m_showHideAnimationFrameLeft >= MENU_DURATION_SHOWHIDE)
      return;

   /* show menu according to animation frame lefts */
   id = m_order[m_currentPos];

   glPushMatrix();

   renderBegin();
   if (m_showHideAnimationFrameLeft > 0.0f) {
      /* sliding animation ad show/hide */
      r = m_showHideAnimationFrameLeft / MENU_DURATION_SHOWHIDE;
      glTranslatef(m_cWidth * r, 0.0f, 0.0f);
   }
   if (m_forwardAnimationFrameLeft > 0.0f) {
      /* moving forward rotation */
      r = m_forwardAnimationFrameLeft / MENU_DURATION_FORWARD;
      glPushMatrix();
      glRotatef(-90.0f + 90.0f * r, MENU_ROTATION_AXIS);
      renderStem(m_order[m_currentPos - 1]);
      glPopMatrix();
      glRotatef(90.0f * r, MENU_ROTATION_AXIS);
   }
   if (m_forwardRegistAnimationFrameLeft > 0.0f) {
      /* forward regist rotation, go and back */
      r = m_forwardRegistAnimationFrameLeft / MENU_DURATION_REGIST;
      r = r * (1.0f - r);
      glRotatef(-30.0f * r, MENU_ROTATION_AXIS);
   }
   if (m_backwardAnimationFrameLeft > 0.0f) {
      /* moving backward rotation */
      r = m_backwardAnimationFrameLeft / MENU_DURATION_BACKWARD;
      glPushMatrix();
      glRotatef(90.0f - 90.0f * r, MENU_ROTATION_AXIS);
      renderStem(m_order[m_currentPos + 1]);
      glPopMatrix();
      glRotatef(-90.0f * r, MENU_ROTATION_AXIS);
   }
   if (m_backwardRegistAnimationFrameLeft > 0.0f) {
      /* backward regist rotation, go and back */
      r = m_backwardRegistAnimationFrameLeft / MENU_DURATION_REGIST;
      r = r * (1.0f - r);
      glRotatef(30.0f * r, MENU_ROTATION_AXIS);
   }
   renderStem(id);
   renderEnd();

   glPopMatrix();
}

/* Menu::procMessage: process message */
bool Menu::processMessage(const char *type, const char *args)
{
   char buff[MMDAGENT_MAXBUFLEN];
   char *argv[6];
   int num = 0;
   char *str1, *str2;
   int id, n;

   /* divide string into arguments */
   if (MMDAgent_strlen(args) > 0) {
      strncpy(buff, args, MMDAGENT_MAXBUFLEN - 1);
      buff[MMDAGENT_MAXBUFLEN - 1] = '\0';
      for (str1 = MMDAgent_strtok(buff, "|", &str2); str1; str1 = MMDAgent_strtok(NULL, "|", &str2)) {
         argv[num] = str1;
         num++;
         if (num >= 5) {
            argv[num] = MMDAgent_strtok(NULL, "\r\n", &str2);
            num++;
            break;
         }
      }
   }
   if (num < 1) {
      m_mmdagent->sendLogString("Error: %s: no arguments", type);
      return false;
   }

   if (MMDAgent_strequal(argv[0], MENU_COMMAND_ADD)) {
      /* MENU|ADD|alias */
      if (num != 2) {
         m_mmdagent->sendLogString("Error: %s|%s: too few arguments", type, argv[0]);
         return false;
      }
      if (find(argv[1]) != -1) {
         m_mmdagent->sendLogString("Warning: %s|%s: alias %s already exists", type, argv[0], argv[1]);
         return false;
      }
      if (add(argv[1], MENUPRIORITY_CONTENT, NULL, NULL) == -1) {
         m_mmdagent->sendLogString("Error: %s|%s: failed to add alias %s", type, argv[0], argv[1]);
         return false;
      }
      m_mmdagent->sendMessage(MENU_EVENT_TYPE, "%s|%s", MENU_COMMAND_ADD, argv[1]);
   } else if (MMDAgent_strequal(argv[0], MENU_COMMAND_DELETE)) {
      /* MENU|DELETE|alias */
      if (num != 2) {
         m_mmdagent->sendLogString("Error: %s|%s: too few arguments", type, argv[0]);
         return false;
      }
      id = find(argv[1]);
      if (id == -1) {
         m_mmdagent->sendLogString("Warning: %s|%s: alias %s not found", type, argv[0], argv[1]);
         return false;
      }
      if (remove(id) == false) {
         m_mmdagent->sendLogString("Error: %s|%s: failed to remove alias %s", type, argv[0], argv[1]);
         return false;
      }
      m_mmdagent->sendMessage(MENU_EVENT_TYPE, "%s|%s", MENU_COMMAND_DELETE, argv[1]);
   } else if (MMDAgent_strequal(argv[0], MENU_COMMAND_SETITEM)) {
      /* MENU|SETITEM|alias|n|label|MESSAGE|.. */
      if (num < 5) {
         m_mmdagent->sendLogString("Error: %s|%s: too few arguments", type, argv[0]);
         return false;
      }
      id = find(argv[1]);
      if (id == -1) {
         m_mmdagent->sendLogString("Error: %s|%s: alias %s not found", type, argv[0], argv[1]);
         return false;
      }
      n = MMDAgent_str2int(argv[2]);
      if (setItem(id, n, argv[3], argv[4], argv[5]) == false) {
         m_mmdagent->sendLogString("Error: %s|%s: failed to add item to alias %s", type, argv[0], argv[1]);
         return false;
      }
      m_mmdagent->sendMessage(MENU_EVENT_TYPE, "%s|%s|%s", MENU_COMMAND_SETITEM, argv[1], argv[2]);
   } else if (MMDAgent_strequal(argv[0], MENU_COMMAND_DELETEITEM)) {
      /* MENU|DELITEM|alias|n */
      if (num != 3) {
         m_mmdagent->sendLogString("Error: %s|%s: too few arguments", type, argv[0]);
         return false;
      }
      id = find(argv[1]);
      if (id == -1) {
         m_mmdagent->sendLogString("Error: %s|%s: alias %s not found", type, argv[0], argv[1]);
         return false;

      }
      n = MMDAgent_str2int(argv[2]);
      if (removeItem(id, n) == false) {
         m_mmdagent->sendLogString("Error: %s|%s: failed to remove item from alias %s", type, argv[0], argv[1]);
         return false;
      }
      m_mmdagent->sendMessage(MENU_EVENT_TYPE, "%s|%s|%s", MENU_COMMAND_DELETEITEM, argv[1], argv[2]);
   }

   return true;
}

