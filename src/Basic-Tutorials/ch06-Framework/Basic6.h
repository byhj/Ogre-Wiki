#ifndef BASIC6_H
#define BASIC6_H

/*
  1.Create the Ogre::Root object
  2.Define the resources that Ogre will use
  3.Choose and set up the RenderSystem (DirectX, OpenGL, etc)
  4.Create the RenderWindow
  5.Set up any third party libraries and plugins.
  6.Initialise resources
  7.Register listener classes
  8.Build a scene
  9.Start the render loop

*/

#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>


class Basic6: public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
	Basic6();
	virtual ~Basic6();

    bool go();

private:
	//Ogre

	Ogre::String m_ResourcesCfg;
	Ogre::String m_PluginsCfg;
	Ogre::Root         *m_pRoot;
	Ogre::RenderWindow *m_pWindow;
	Ogre::SceneManager *m_pSceneMgr;
	Ogre::Camera       *m_pCamera;

	//OIS
	OIS::InputManager* m_pInputManager;
	OIS::Mouse       * m_pMouse;
	OIS::Keyboard    * m_pKeyboard;

	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

#endif // !BASIC6_H
