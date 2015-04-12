#include "Mch6.h"

BasicApp::BasicApp()
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
	mInputMgr(0),
	mAnim(0),
	mProjectorNode(0),
	mDecalFrustum(0),
	mFilterFrustum(0)
{
}

BasicApp::~BasicApp()
{
	if (mCameraMan) delete mCameraMan;

	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);

	delete mRoot;
}

void BasicApp::go()
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

bool BasicApp::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
		mShutdown = true;

	if (mShutdown)
		return false;

	if (mWindow->isClosed())
		return false;

	mKeyboard->capture();
	mMouse->capture();

	mCameraMan->frameRenderingQueued(fe);

	mProjectorNode->rotate(
		Ogre::Vector3::UNIT_Y,
		Ogre::Degree(fe.timeSinceLastFrame * 10));

	CEGUI::Window* tutorialLabel =
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild(
		"SampleLabel");

	CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

	return true;
}

bool BasicApp::keyPressed(const OIS::KeyEvent& ke)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectKeyDown((CEGUI::Key::Scan)ke.key);
	// context.injectChar((CEGUI::Key::Scan)ke.text);

	mCameraMan->injectKeyDown(ke);

	return true;
}

bool BasicApp::keyReleased(const OIS::KeyEvent& ke)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectKeyUp((CEGUI::Key::Scan)ke.key);

	mCameraMan->injectKeyUp(ke);

	return true;
}

bool BasicApp::mouseMoved(const OIS::MouseEvent& me)
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

bool BasicApp::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectMouseButtonDown(convertButton(id));

	mCameraMan->injectMouseDown(me, id);

	return true;
}

bool BasicApp::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	// CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	// context.injectMouseButtonUp(convertButton(id));

	mCameraMan->injectMouseUp(me, id);

	return true;
}

void BasicApp::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState& ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void BasicApp::windowClosed(Ogre::RenderWindow* rw)
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

bool BasicApp::setup()
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

bool BasicApp::configure()
{
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	mWindow = mRoot->initialise(true, "ITutorial");

	return true;
}

void BasicApp::chooseSceneManager()
{
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}

void BasicApp::createCamera()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(0, 0, 80));
	mCamera->lookAt(Ogre::Vector3(0, 0, -300));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void BasicApp::createScene()
{
	// mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.3, 0.5));

	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(0, 40, 0);

	mCamera->setPosition(60, 100, 80);
	mCamera->lookAt(0, 70, 0);

	// Add floor plane
	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;

	Ogre::MeshManager::getSingleton().createPlane(
		"floor",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		450.0, 450.0,
		10, 10, true, 1,
		50.0, 50.0,
		Ogre::Vector3::UNIT_Z);

	Ogre::Entity* planeEntity = mSceneMgr->createEntity("floor");
	planeEntity->setMaterialName("Tutorial/SpaceFloor");
	planeEntity->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEntity);

	// Add Ogre heads
	int numOfHeads = 14;
	double radius = 180;
	Ogre::Radian curAngle(0);
	Ogre::Entity* ogreEntity;

	for (int i = 0; i < numOfHeads; ++i)
	{
		Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
		headNode->setScale(2.2, 2.8, 2.2);
		headNode->attachObject(ogreEntity);

		curAngle = curAngle + Ogre::Radian(Ogre::Math::TWO_PI / numOfHeads);

		headNode->setPosition(
			-radius * Ogre::Math::Sin(curAngle),
			20,
			-radius * Ogre::Math::Cos(curAngle));

		headNode->rotate(Ogre::Vector3::UNIT_Y, curAngle);
	}

	createProjector();

	for (unsigned int i = 0; i < planeEntity->getNumSubEntities(); ++i)
		makeMaterialReceiveDecal(planeEntity->getSubEntity(i)->getMaterialName());

	for (unsigned int i = 0; i < ogreEntity->getNumSubEntities(); ++i)
		makeMaterialReceiveDecal(ogreEntity->getSubEntity(i)->getMaterialName());

	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* rootWin = wmgr.loadLayoutFromFile("test.layout");

	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(rootWin);

}

void BasicApp::destroyScene()
{
}

void BasicApp::createFrameListener()
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

void BasicApp::createViewports()
{
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
}

void BasicApp::setupResources()
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

void BasicApp::createResourceListener()
{
}

void BasicApp::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool BasicApp::setupCEGUI()
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

void BasicApp::createProjector()
{
	mDecalFrustum = new Ogre::Frustum();

	mProjectorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mProjectorNode->attachObject(mDecalFrustum);
	mProjectorNode->setPosition(0, 30, 0);

	mFilterFrustum = new Ogre::Frustum();
	mFilterFrustum->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

	Ogre::SceneNode* filterNode = mProjectorNode->createChildSceneNode();
	filterNode->attachObject(mFilterFrustum);
	filterNode->setOrientation(
		Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));

}

void BasicApp::addDecalToMaterial(const Ogre::String& matName)
{
	Ogre::MaterialPtr mat =
		(Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(matName);

	Ogre::Pass* pass = mat->getTechnique(0)->createPass();
	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	pass->setDepthBias(1);
	pass->setLightingEnabled(false);

	Ogre::TextureUnitState* texState = pass->createTextureUnitState("decal.png");
	texState->setProjectiveTexturing(true, mDecalFrustum);
	texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	texState->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);

	texState = pass->createTextureUnitState("decal_filter.png");
	texState->setProjectiveTexturing(true, mFilterFrustum);
	texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	texState->setTextureFiltering(Ogre::TFO_NONE);
}