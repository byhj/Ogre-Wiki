#ifndef BCH5_H
#define BCH5_H

#include "OgreWiki.h"

class BCH5: public BaseApplication {
public:
	BCH5(void);
	virtual ~BCH5(void); //当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用。
protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);//帧监听
	//key
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	virtual bool keyPressed(const OIS::KeyEvent &evt);
	virtual bool keyReleased(const OIS::KeyEvent &evt);
	//mouse
	virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
private:
	Ogre::Real mRotate;
	Ogre::Real mMove;
	Ogre::SceneNode *mCamNode;
	Ogre::Vector3 mDirection;
};
	

#endif