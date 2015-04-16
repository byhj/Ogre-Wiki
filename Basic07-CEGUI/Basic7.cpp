#include "Basic7.h"

Basic7::Basic7(void)
{
}

Basic7::~Basic7(void)
{
}

void Basic7::createScene(void)
{
	//set the cegui resources
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	//set the cegui window informatoin
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

	//add the quit label
	sheet->addChild(quit);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked,  CEGUI::Event::Subscriber(&Basic7::quit, this));

	//create a test scene
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0, 0, -300));
	headNode->attachObject(ogreHead);

	//Render to the texture
	Ogre::TexturePtr tex = mRoot->getTextureManager()->createManual(
		"RTT",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D,
		512,
		512,
		0,
		Ogre::PF_R8G8B8,
		Ogre::TU_RENDERTARGET);
	Ogre::RenderTexture *rtex = tex->getBuffer()->getRenderTarget();

	//create a camera for rtt
	Ogre::Camera *cam = mSceneMgr->createCamera("RTTCam");
	cam->setPosition(100, -100, -400);
	cam->lookAt(0, 0, -300);
	Ogre::Viewport *v = rtex->addViewport(cam);
	v->setOverlaysEnabled(false);
	v->setClearEveryFrame(true);
	v->setBackgroundColour(Ogre::ColourValue::Black);

	CEGUI::Texture &guiTex = mRenderer->createTexture("textname", tex);

	const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
	CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
	image->setTexture(&guiTex);
	image->setArea(rect);
	image->setAutoScaled(CEGUI::ASM_Both);

	CEGUI::Window *si = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage", "RTTWindow");
	//si->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),
	si->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0),
		CEGUI::UDim(0.4f, 0)));
	si->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),
		CEGUI::UDim(0.0f, 0)));
	si->setProperty("Image", "RTTImage");
	sheet->addChild(si);
}

void Basic7::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);
	windowResized(mWindow);

	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	mRoot->addFrameListener(this);

}

bool Basic7::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	//Need to inject timestamps to CEGUI System.
	CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	return true;
}

//ceguit mouse and key 
bool Basic7::keyPressed(const OIS::KeyEvent &arg)
{
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown ((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);
	return true;
}

bool Basic7::keyReleased(const OIS::KeyEvent &arg)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
	return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;
 
    default:
        return CEGUI::LeftButton;
        break;
    }
}

bool Basic7::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id)))
    mCameraMan->injectMouseDown(arg, id);
    return true;
}

bool Basic7::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
	return true;
}

bool Basic7::mouseMoved(const OIS::MouseEvent &arg)
{ 
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);

	return true;
}
bool Basic7::quit(const CEGUI::EventArgs &e)
{
    mShutDown = true;
    return true;
}