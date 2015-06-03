#ifndef BASIC4
#define BASIC4

#include "OgreWiki.h"

class Basic4: public OgreWiki::Application
{
public:
	Basic4();
	virtual ~Basic4();

protected:
	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

private:
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);

};
#endif