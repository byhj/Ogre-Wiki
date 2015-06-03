#ifndef __Basic8_h_
#define __Basic8_h_

#include "OgreWiki.h"

class Basic8 : public OgreWiki::Application
{
public:
	Basic8(void);
	virtual ~Basic8(void);

protected:
	virtual void createScene(void);
	virtual void chooseSceneManager(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);


	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

private:
	//We create two scene scenemanager
	Ogre::SceneManager* mPrimarySceneMgr;
	Ogre::SceneManager* mSecondarySceneMgr;
	bool mDual;

	virtual void setupViewport(Ogre::SceneManager *curr);
	virtual void dualViewport(Ogre::SceneManager *primarySceneMgr, Ogre::SceneManager *secondarySceneMgr);
};

#endif // #ifndef