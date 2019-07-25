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

#define PMDOBJECT_ALPHAFRAME    15.0      /* number of frames for appear/disappear alpha blending */
#define PMDOBJECT_MINMOVEDIFF   0.0001f
#define PMDOBJECT_MOVESPEEDRATE 0.90f     /* current * 0.90 + target * 0.10 */
#define PMDOBJECT_MINSPINDIFF   0.000001f
#define PMDOBJECT_SPINSPEEDRATE 0.95f     /* current * 0.95 + target * 0.05 */

/* PMDObject: object of PMD */
class PMDObject
{
private:

   char *m_alias;                  /* alias */
   PMDModel m_pmd;                 /* model */
   MotionManager *m_motionManager; /* motion manager */

   LipSync *m_globalLipSync; /* lip sync */
   LipSync *m_localLipSync;

   bool m_isEnable; /* true if this model is enabled */

   /* status and work area */
   btVector3 m_lightDir; /* light direction for toon shading */

   /* model configuration */
   PMDObject *m_assignTo;   /* parent model when this is accessory */
   PMDBone *m_baseBone;     /* parent bone when this is accessory */
   btVector3 m_origBasePos; /* offset when position is fixed */

   btVector3 m_offsetPos;      /* root bone offset for accessory or moving */
   btQuaternion m_offsetRot;   /* root bone rotation for accessory or moving */
   bool m_absPosFlag[3];       /* absolute position flag for accessory per each axis */
   float m_moveSpeed;          /* move speed per second: if negative value, warp to m_offsetPos */
   float m_spinSpeed;          /* spin speed per second: if negative value, warp to m_offsetRot */
   bool m_useCartoonRendering; /* false if deny cartoon rendering */
   bool m_allowMotionFileDrop; /* true if allow motion file drop or all motion command */

   bool m_isMoving;   /* true when model move */
   bool m_isRotating; /* true when model spin */
   bool m_underTurn;  /* true when model turn */

   double m_alphaAppearFrame;    /* number of alpha frame when model appear */
   double m_alphaDisappearFrame; /* number of alpha frame when model disapper */
   double m_displayCommentFrame; /* number of frame to show comment ofs model */

   bool m_needResetKinematic; /* flag for reset Kinematic State when base motion is changed */

   FTGLTextDrawElements m_commentElem;   /* comment element */
   FTGLTextDrawElements m_errorElem;     /* error element */
   FTGLTextDrawElements m_nameElem;      /* name element */

   /* initialize: initialize PMDObject */
   void initialize();

   /* clear: free PMDObject */
   void clear();

public:

   /* PMDObject: constructor */
   PMDObject();

   /* PMDObject: destructor */
   ~PMDObject();

   /* release: free PMDObject */
   void release();

   /* load: load model */
   bool load(const char *fileName, const char *alias, btVector3 *offsetPos, btQuaternion *offsetRot, bool forcedPosition, PMDBone *assignBone, PMDObject *assignObject, BulletPhysics *bullet, SystemTexture *systex, LipSync *sysLipSync, bool useCartoonRendering, float cartoonEdgeWidth, btVector3 *light, float commentFrame);

   /* skipNextSimulation: skip next physics simulation */
   void skipNextSimulation();

   /* setMotion: start a motion */
   bool startMotion(VMD *vmd, const char *name, bool full, bool once, bool enableSmooth, bool enableRepos, float priority);

   /* swapMotion: swap a motion */
   bool swapMotion(VMD *vmd, const char *name);

   /* updateRootBone: update root bone if assigned to a base bone */
   void updateRootBone();

   /* updateMotion: update motions */
   bool updateMotion(double deltaFrame);

   /* updateAfterSimulation: update bone transforms from simulated rigid bodies */
   void updateAfterSimulation(bool physicsEnabled);

   /* updateSkin: update skin and toon */
   void updateSkin();

   /* updateAlpha: update global model alpha */
   bool updateAlpha(double deltaFrame);

   /* startDisppear: set disapper timer */
   void startDisappear();

   /* setLightForToon: set light direction for ton shading */
   void setLightForToon(btVector3 *v);

   /* updateModel: update model position of root bone */
   bool updateModelRootOffset(float fps);

   /* updateModelRootRotation: update model rotation of root bone */
   bool updateModelRootRotation(float fps);

   /* getAlias: get alias name */
   char *getAlias();

   /* setAlias: set alias name */
   void setAlias(const char *alias);

   /* getPMDModel: get PMDModel */
   PMDModel *getPMDModel();

   /* getMotionManager: get MotionManager */
   MotionManager *getMotionManager();

   /* resetMotionManager: reset MotionManager */
   void resetMotionManager();

   /* createLipSyncMotion: create LipSync motion */
   bool createLipSyncMotion(const char *str, unsigned char **rawData, unsigned int *rawSize);

   /* getPosition: get current offset */
   void getCurrentPosition(btVector3 *pos);

   /* getTargetPosition: get target offset */
   void getTargetPosition(btVector3 *pos);

   /* setPosition: set root bone offset */
   void setPosition(btVector3 *pos);

   /* getRotation: get current rotation */
   void getCurrentRotation(btQuaternion *rot);

   /* getTargetRotation: get target rotation */
   void getTargetRotation(btQuaternion *rot);

   /* setRotation: set root bone rotation */
   void setRotation(btQuaternion *rot);

   /* setMoveSpeed: set move speed per second */
   void setMoveSpeed(float speed);

   /* setSpinSpeed: set spin seed per second */
   void setSpinSpeed(float speed);

   /* isMoving: return true when model move */
   bool isMoving();

   /* isRotating: return true when model spin */
   bool isRotating();

   /* isTruning: return true when model turn */
   bool isTurning();

   /* setTurnFlag: set turnning flag */
   void setTurningFlag(bool flag);

   /* isEnable: get enable flag */
   bool isEnable();

   /* setEnableFlag: set enable flag */
   void setEnableFlag(bool flag);

   /* useCartoonRendering: return true if cartoon rendering is enabled */
   bool useCartoonRendering();

   /* allowMotionFileDrop: return true if motion file drop is allowed */
   bool allowMotionFileDrop();

   /* getAssignedModel: get parent model */
   PMDObject *getAssignedModel();

   /* renderText: render model name, comment and error text */
   void renderText(FTGLTextureFont *font, bool displayModelNameFlag);

   /* renderModelDebug: render model debug */
   void renderModelDebug();
};
