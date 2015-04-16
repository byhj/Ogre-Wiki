#ifndef Basic4_H
#define Basic4_H

#include "OgreWiki.h"

class Basic4 : public BaseApplication {
public:
	Basic4(void);
	virtual ~Basic4(void);
protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
private:
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);
};

#endif