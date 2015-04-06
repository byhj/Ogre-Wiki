/*
The basic Ogre life cycle looks like this:

1:Create the Root object.
2:Define the resources that Ogre will use.
3:Choose and set up the RenderSystem (that is, DirectX, OpenGL, etc).
4:Create the RenderWindow (the window which Ogre resides in).
5:Initialise the resources that you are going to use.
6:Create a scene using those resources.
7:Set up any third party libraries and plugins.
8:Create any number of frame listeners.
9:Start the render loop.

*/

#ifndef BCH6_H
#define BCH6_H

#include <OgreRoot.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>
#include <OgreTextureManager.h>
#include <OgreSceneNode.h>
#include <OgreLogManager.h>

class BCH6 :public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	BCH6(void);
	virtual ~BCH6(void);
	bool go(void);
protected:
    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
 
    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
    Ogre::Root* mRoot;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::RenderWindow* mWindow;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
 
    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
};

#endif