#ifndef CH4_H
#define CH4_H

#include "BaseApplication.h"

class CH4 : public BaseApplication {
public:
	CH4(void);
	virtual ~CH4(void);
protected:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
private:
	bool processUnbufferedInput(const Ogre::FrameEvent &evt);
};

#endif