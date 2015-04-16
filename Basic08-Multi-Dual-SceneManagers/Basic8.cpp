#include "Basic8.h"
#define CAMERA_NAME "SceneCamera"

//-------------------------------------------------------------------------------------
Basic8::Basic8(void)
	:mPrimarySceneMgr(0),
	mSecondarySceneMgr(0),
	mDual(false)
{
}
//-------------------------------------------------------------------------------------
Basic8::~Basic8(void)
{
}


//-------------------------------------------------------------------------------------

//Local Functions, set the viewport
void Basic8::setupViewport(Ogre::SceneManager *curr)
{
	mWindow->removeAllViewports();

	Ogre::Camera *cam = curr->getCamera(CAMERA_NAME);
	Ogre::Viewport *vp = mWindow->addViewport(cam);

	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

//set the double viewport
void Basic8::dualViewport(Ogre::SceneManager *primarySceneMgr, Ogre::SceneManager *secondarySceneMgr)
{
	mWindow->removeAllViewports();

	Ogre::Viewport *vp = 0;
	Ogre::Camera *cam = primarySceneMgr->getCamera(CAMERA_NAME);
	vp = mWindow->addViewport(cam, 0, 0, 0, 0.5, 1);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	cam = secondarySceneMgr->getCamera(CAMERA_NAME);
	vp = mWindow->addViewport(cam, 1, 0.5, 0, 0.5, 1);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}

//swap the sceneManager to get different scene
static void swap(Ogre::SceneManager *&first, Ogre::SceneManager *&second)
{
	Ogre::SceneManager *tmp = first;
	first = second;
	second = tmp;
}


//-------------------------------------------------------------------------------------
void Basic8::createScene(void)
{
	// Setup the TerrainSceneManager
	mPrimarySceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	// Setup the Generic SceneManager
	mSecondarySceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
}


void Basic8::chooseSceneManager(void)
{
	mPrimarySceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "primary");
	mSecondarySceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "secondary");
}

void Basic8::createCamera()
{
	mPrimarySceneMgr->createCamera(CAMERA_NAME);
	mSecondarySceneMgr->createCamera(CAMERA_NAME);
}

void Basic8::createViewports()
{
	setupViewport(mPrimarySceneMgr);
}


void Basic8::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
}


bool Basic8::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	return true;
}


bool Basic8::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_ESCAPE){
		mShutDown = true;
	}
	else if(arg.key == OIS::KC_V){
		mDual = !mDual;

		if (mDual)
			dualViewport(mPrimarySceneMgr, mSecondarySceneMgr);
		else
			setupViewport(mPrimarySceneMgr);
	}
	else if(arg.key == OIS::KC_C){
		swap(mPrimarySceneMgr, mSecondarySceneMgr);

		if (mDual)
			dualViewport(mPrimarySceneMgr, mSecondarySceneMgr);
		else
			setupViewport(mPrimarySceneMgr);
	}

	return true;
}

bool Basic8::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}

bool Basic8::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

bool Basic8::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

bool Basic8::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}