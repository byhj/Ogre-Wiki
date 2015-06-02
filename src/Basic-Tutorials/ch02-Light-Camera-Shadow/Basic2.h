#ifndef BASIC2_H
#define BASIC2_H

#include "OgreWiki.h"

class Basic2: public OgreWiki::Application
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