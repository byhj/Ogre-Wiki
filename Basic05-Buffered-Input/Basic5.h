#ifndef Basic5_H
#define Basic5_H

#include "OgreWiki.h"

class Basic5 : public BaseApplication
{
public:
	Basic5();
	virtual ~Basic5();

private:
	virtual void createScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

	virtual bool keyPressed(const OIS::KeyEvent& ke);
	virtual bool keyReleased(const OIS::KeyEvent& ke);

	virtual bool mouseMoved(const OIS::MouseEvent& me);
	virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::SceneNode* mCamNode;
	Ogre::Vector3 mDirection;

};

#endif