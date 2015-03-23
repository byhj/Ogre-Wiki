#ifndef MCH7_H
#define MCH7_H

#include "BaseApplication.h"

class Mch7: public BaseApplication
{
public:
	Mch7(void);
	virtual ~Mch7(void);
protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);  
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);

	Ogre::MovablePlane *mPlane;
	Ogre::Entity *mPlaneEnt;
	Ogre::SceneNode *mPlaneNode;
	Ogre::Rectangle2D *mMiniScreen;

};

#endif