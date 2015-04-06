#ifndef BCH4_H
#define BCH4_H

#include "OgreWiki.h"

class BCH4 : public BaseApplication {
public:
	BCH4(void);
	virtual ~BCH4(void);
protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
private:
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);
};

#endif