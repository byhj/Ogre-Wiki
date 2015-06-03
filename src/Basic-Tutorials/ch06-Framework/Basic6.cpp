#include "Basic6.h"


Basic6::Basic6()
	:
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR < 10
	m_ResourcesCfg(Ogre::StringUtil::BLANK),
	m_PluginsCfg(Ogre::StringUtil::BLANK),
#else
	m_ResourcesCfg(Ogre::BLANKSTRING),
	m_PluginsCfg(Ogre::BLANKSTRING),
#endif
	m_pRoot(0),
	m_pWindow(0),
	m_pSceneMgr(0),
	m_pCamera(0)
{

}

Basic6::~Basic6()
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(m_pWindow, this);
	windowClosed(m_pWindow);
	delete m_pRoot;
}


bool Basic6::go()
{
#ifdef _DEBUG
	m_ResourcesCfg = "resources_d.cfg";
	m_PluginsCfg = "plugins_d.cfg";
#else
	m_ResourcesCfg = "resources.cfg";
	m_PluginsCfg = "plugins.cfg";
#endif

	////////////////1.Creating the Root Object//////////////////////////////
	m_pRoot = new Ogre::Root(m_PluginsCfg);

	///////////////2.Setting Up Resources///////////////////////////////
	Ogre::ConfigFile cf;
	cf.load(m_ResourcesCfg);

	//The name parameter is the path to the resources (i.e. "../media"). 
	//The locType parameter defines what kind of location this is (i.e. Filesystem, Zip, etc.) 
	Ogre::String name, locType;

	//Only let ogre know where to look resource, not yet initialize
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator(); 
	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;
	    for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
		}
	}

	/////////////////3.Configuring the RenderSystem//////////////////////
	if(!(m_pRoot->restoreConfig() || m_pRoot->showConfigDialog()))
		return false;
	m_pWindow = m_pRoot->initialise(true, "Ogre Render Window");


	///////////////4.Initialising Resources////////////////////////
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	///////////////5.Creating a SceneManager//////////////////////
	m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_GENERIC);

	//////////////6.Create a Camera/////////////////////
	m_pCamera = m_pSceneMgr->createCamera("PlayerCam");
	m_pCamera->setPosition(0, 0, 80);
	m_pCamera->lookAt(0, 0, 0);
	m_pCamera->setNearClipDistance(5);

	//////////////////Add Viewport///////////////////////
	Ogre::Viewport* vp = m_pWindow->addViewport(m_pCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	m_pCamera->setAspectRatio(
		   Ogre::Real(vp->getActualWidth()) / 
		   Ogre::Real(vp->getActualHeight()));

	//////////////////Setting up the scene ////////////////
	Ogre::Entity* ogreEntity = m_pSceneMgr->createEntity("ogrehead.mesh");

	Ogre::SceneNode* ogreNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light* pLight = m_pSceneMgr->createLight("MainLight");
	pLight->setPosition(20, 80, 50);



	////////////////Initial OIS/////////////////////
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	m_pWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	m_pInputManager = OIS::InputManager::createInputSystem( pl );

	//We are passing 'false' to the createInputObject function because we want mouse and keyboard unbuffered. 
	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputManager->createInputObject( OIS::OISKeyboard, false ));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputManager->createInputObject( OIS::OISMouse, false ));

	//Set initial mouse clipping size
	windowResized(m_pWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(m_pWindow, this);
	m_pRoot->addFrameListener(this);
	m_pRoot->startRendering();

    return true;
}

//Adjust mouse clipping area
void Basic6::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = m_pMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void Basic6::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if(rw == m_pWindow)
	{
		if(m_pInputManager)
		{
			m_pInputManager->destroyInputObject( m_pMouse );
			m_pInputManager->destroyInputObject( m_pKeyboard );

			OIS::InputManager::destroyInputSystem(m_pInputManager);
			m_pInputManager = 0;
		}
	}
}

bool Basic6::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(m_pWindow->isClosed())
		return false;

	//Need to capture/update each device
	m_pKeyboard->capture();
	m_pMouse->capture();

	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
		return false;

	return true;
}