#ifndef BCH2_H
#define BCH2_H

#include "OgreWiki.h"

class BCH2:public BaseApplication
{
public:
   BCH2(void);
   virtual ~BCH2(void);
protected:
	virtual void createScene();
	virtual void createCamera();
	virtual void createViewports();
};

#endif