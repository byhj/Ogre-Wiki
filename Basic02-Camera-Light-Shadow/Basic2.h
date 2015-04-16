#ifndef _BASIC1_H_ 
#define _BASIC1_H_

#include "OgreWiki.h"

class Basic2: public BaseApplication
{
public:
	Basic2(void);
	virtual ~Basic2(void);

protected:
	virtual void createScene();
	virtual void createCamera();
	virtual void createViewports();
};

#endif