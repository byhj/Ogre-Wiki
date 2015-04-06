#pragma once

#include "OgreWiki.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class BCH7 :public BaseApplication
{
public:
	BCH7(void);
	virtual ~BCH7(void);
protected:
	CEGUI::OgreRenderer *mRenderer;
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	//key listener
	virtual bool keyPressed(const OIS::KeyEvent &arg);
	virtual bool keyReleased(const OIS::KeyEvent &arg);

	//mouse listener
	virtual bool mouseMoved(const OIS::MouseEvent &arg);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	bool quit(const CEGUI::EventArgs &s);
};

