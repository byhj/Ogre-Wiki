#include "Middle7.h"

Middle7::Middle7()
	: mShutdown(false),
	mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mCameraMan(0),
	mRenderer(0),
	mMouse(0),
	mKeyboard(0),
	mInputMgr(0)
{
}

Middle7::~Middle7()
{
	if (mCameraMan) delete mCameraMan;

	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}

void Middle7::go()
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return;

	mRoot->startRendering();

	destroyScene();
}

bool Middle7::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		mShutdown = true;

	if (mShutdown)
		return false;

	if (mWindow->isClosed())
		return false;

	mKeyboard->capture();
	mMouse->capture();

	mPlaneNode->yaw(Ogre::Radian(0.5 * fe.timeSinceLastFrame));

	mCameraMan->frameRenderingQueued(fe);

	CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

	return true;
}

bool Middle7::keyPressed(const OIS::KeyEvent& ke)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectKeyDown((CEGUI::Key::Scan)ke.key);
	// context.injectChar((CEGUI::Key::Scan)ke.text);

	mCameraMan->injectKeyDown(ke);

	return true;
}

bool Middle7::keyReleased(const OIS::KeyEvent& ke)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectKeyUp((CEGUI::Key::Scan)ke.key);

	mCameraMan->injectKeyUp(ke);

	return true;
}

bool Middle7::mouseMoved(const OIS::MouseEvent& me)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectMouseMove(me.state.X.rel, me.state.Y.rel);

	mCameraMan->injectMouseMove(me);

	return true;
}

// Helper function for mouse events
CEGUI::MouseButton convertButton(OIS::MouseButtonID id)
{
	switch (id)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
	case OIS::MB_Right:
		return CEGUI::RightButton;
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
	default:
		return CEGUI::LeftButton;
	}
}

bool Middle7::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectMouseButtonDown(convertButton(id));

	mCameraMan->injectMouseDown(me, id);

	return true;
}

bool Middle7::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectMouseButtonUp(convertButton(id));

	mCameraMan->injectMouseUp(me, id);

	return true;
}

void Middle7::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void Middle7::windowClosed(Ogre::RenderWindow* rw)
{
	if (rw == mWindow)
	{
		if (mInputMgr)
		{
			mInputMgr->destroyInputObject(mMouse);
			mInputMgr->destroyInputObject(mKeyboard);

			OIS::InputManager::destroyInputSystem(mInputMgr);
			mInputMgr = 0;
		}
	}
}

bool Middle7::setup()
{
	mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	if (!configure())
		return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	createResourceListener();
	loadResources();

	setupCEGUI();

	createScene();

	createFrameListener();

	return true;
}

bool Middle7::configure()
{
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	mWindow = mRoot->initialise(true, "ITutorial");

	return true;
}

void Middle7::chooseSceneManager()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

void Middle7::createCamera()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(0, 0, 80));
	mCamera->lookAt(Ogre::Vector3(0, 0, -300));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void Middle7::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	mCamera->setPosition(60, 200, 70);
	mCamera->lookAt(0,0,0);

	Ogre::MaterialPtr mat =
		Ogre::MaterialManager::getSingleton().create(
		"PlaneMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::TextureUnitState* tuisTexture =
		mat->getTechnique(0)->getPass(0)->createTextureUnitState("grass_1024.jpg");

	mPlane = new Ogre::MovablePlane("Plane");
	mPlane->d = 0;
	mPlane->normal = Ogre::Vector3::UNIT_Y;

	Ogre::MeshManager::getSingleton().createPlane(
		"PlaneMesh",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		*mPlane,
		120, 120, 1, 1,
		true,
		1, 1, 1,
		Ogre::Vector3::UNIT_Z);
	mPlaneEntity = mSceneMgr->createEntity("PlaneMesh");
	mPlaneEntity->setMaterialName("PlaneMat");

	mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mPlaneNode->attachObject(mPlaneEntity);

	Ogre::TexturePtr rttTexture =
		Ogre::TextureManager::getSingleton().createManual(
		"RttTex",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		mWindow->getWidth(), mWindow->getHeight(),
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);

	Ogre::RenderTexture* renderTexture = rttTexture->getBuffer()->getRenderTarget();
	renderTexture->addViewport(mCamera);
	renderTexture->getViewport(0)->setClearEveryFrame(true);
	renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
	renderTexture->getViewport(0)->setOverlaysEnabled(false);

	renderTexture->update();
	renderTexture->writeContentsToFile("start.png");

	mMiniscreen = new Ogre::Rectangle2D(true);
	mMiniscreen->setCorners(.5, 1.0, 1.0, .5);
	mMiniscreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

	Ogre::SceneNode* miniscreenNode =
		mSceneMgr->getRootSceneNode()->createChildSceneNode();
	miniscreenNode->attachObject(mMiniscreen);

	Ogre::MaterialPtr renderMat =
		Ogre::MaterialManager::getSingleton().create(
		"RttMat",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	renderMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	renderMat->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

	mMiniscreen->setMaterial("RttMat");

	renderTexture->addListener(this);

	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);

}

void Middle7::destroyScene()
{
}

void Middle7::createFrameListener()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");

	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(
		mInputMgr->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(
		mInputMgr->createInputObject(OIS::OISMouse, true));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	windowResized(mWindow);

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	Ogre::LogManager::getSingletonPtr()->logMessage("Finished");
}

void Middle7::createViewports()
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
}

void Middle7::setupResources()
{
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

	while (secIt.hasMoreElements())
	{
		secName = secIt.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator setIt;

		for (setIt = settings->begin(); setIt != settings->end(); ++setIt)
		{
			typeName = setIt->first;
			archName = setIt->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

void Middle7::createResourceListener()
{
}

void Middle7::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool Middle7::setupCEGUI()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing CEGUI ***");

	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

	context.setDefaultFont("DejaVuSans-10");
	context.getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	Ogre::LogManager::getSingletonPtr()->logMessage("Finished");

	return true;
}

void Middle7::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	mMiniscreen->setVisible(false);
}

void Middle7::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
	mMiniscreen->setVisible(true);
}