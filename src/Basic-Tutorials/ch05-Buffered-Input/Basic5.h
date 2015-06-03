#ifndef BASIC5
#define BASIC5

#include "OgreWiki.h"

class Basic5: public OgreWiki::Application
{
public:
	Basic5();
	virtual ~Basic5();

private:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool keyPressed(const OIS::KeyEvent &ke);
	virtual bool keyReleased(const OIS::KeyEvent &ke);
	virtual bool mouseMoved(const OIS::MouseEvent &me);
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::SceneNode *pmCamNode;
	Ogre::Vector3 mDirection;
};
#endif // !BASIC5
