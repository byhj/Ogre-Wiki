
#ifndef __Bch8_h_
#define __Bch8_h_

#include "OgreWiki.h"

class BCH8 : public BaseApplication
{
public:
	BCH8(void);
	virtual ~BCH8(void);

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
	Ogre::SceneManager* mPrimarySceneMgr;
	Ogre::SceneManager* mSecondarySceneMgr;
	bool mDual;

	virtual void setupViewport(Ogre::SceneManager *curr);
	virtual void dualViewport(Ogre::SceneManager *primarySceneMgr, Ogre::SceneManager *secondarySceneMgr);
};

#endif // #ifndef