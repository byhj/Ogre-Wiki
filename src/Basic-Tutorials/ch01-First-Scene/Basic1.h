#ifndef BASIC1_H
#define BASIC1_H

#include "OgreWiki.h"

class Basic1: public OgreWiki::Application
{
public:
	Basic1(void);
	virtual ~Basic1(void);

protected:
	virtual void createScene();
};

#endif