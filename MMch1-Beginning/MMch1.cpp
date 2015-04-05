/*
STEP 1/ First, we will need to create the Ogre::Root object. 
It is an object that must be created to use ogre correctly, 
and delete once we are finished using Ogre.

STEP 2/ Then we need to load plugins. It means that there are functions that are stored inside dynamic libraries. 
These libraries are .dll or .so files. Most projects Ogre Project do not need all functions to be usable. 
That way, only a subset of all function can be loaded. It also means you can create your own plugins if you want. 
If you want to know more on the subject, you 'll need to dig into a C++ tutorial. Anyway, for our use, 
we will need to load at least a 'RenderSystem' plugin, which means something to drive opengl or directx. 
The basic plugins you are the most likely to use are the RenderSystems, the particle FX and the Cgprogram.

STEP 3/ Then, we can select from the loaded plugins the unique RenderSystem we want to use.

STEP 4/ When the RenderSystem is selected, we can initialise the Root. 
The root can be initialised only when a rendersystem has been selected.

STEP 5/ Then we can ask to the RenderSystem to create a window.
*/

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/OgreWindowEventUtilities.h>

#include <memory>
#include <exception>

int main()
{

	try {
	    // step 1：Create Ogre::Root object
		Ogre::String lConfigFileName = "";
		Ogre::String lPluginsFileName = "";
		Ogre::String lLogFileName = "Tutorial_Ogre.LOG";
		std::auto_ptr<Ogre::Root> lRoot(new Ogre::Root(lConfigFileName, lPluginsFileName, lLogFileName));	//创建场景根节点
		
		//step 2:load the plugin
		typedef std::vector<Ogre::String> Strings;
		Strings lPluginNames;
		lPluginNames.push_back("RenderSystem_GL");
		lPluginNames.push_back("Plugin_ParticleFX");
		lPluginNames.push_back("Plugin_CgProgramManager");
		lPluginNames.push_back("Plugin_OctreeSceneManager");
		for (auto liter = lPluginNames.begin(); liter != lPluginNames.end(); ++liter)
		{
			Ogre::String &lPluginName = (*liter);
			bool lIsInDebugMode = OGRE_DEBUG_MODE;  
			if (lIsInDebugMode)                  //debug mode
			{
				lPluginName.append("_d");
			}
			lRoot->loadPlugin(lPluginName);     //load the plugin to root node
		}

		//step 3:create the render system
		const Ogre::RenderSystemList & lRenderSystemList = lRoot->getAvailableRenderers();
		if (lRenderSystemList.empty())
		{
			Ogre::LogManager::getSingleton().logMessage("Sorry, no rendersystem was found.");
			return 1;
		}
		Ogre::RenderSystem *lRenderSystem = lRenderSystemList[0];
		lRoot->setRenderSystem(lRenderSystem);

		//step 4:create a window
		bool lCreateAWindowAutoMatically = false;
		Ogre::String lWindowTitle = "";
		Ogre::String lCustomCapacities = "";
		lRoot->initialise(lCreateAWindowAutoMatically, lWindowTitle, lCustomCapacities);
		
		//step 5:ask render system to create a window
		Ogre::RenderWindow * lWindow = NULL;
		lWindowTitle = "Hello Ogre World";
		unsigned int lSizeX = 800;
		unsigned int lSizeY = 600;
		bool lFullscreen = false;
		Ogre::NameValuePairList lParams;
		lParams["FSAA"] = "0";
		lParams["vsync"] = "true";
		lWindow = lRoot->createRenderWindow(lWindowTitle, lSizeX, lSizeY, lFullscreen, &lParams);
	
		//step 6:while until window should close
		while (!lWindow->isClosed())
		{
			Ogre::WindowEventUtilities::messagePump();
		}
		Ogre::LogManager::getSingleton().logMessage("End of the program");
		// catch the exception
	}catch(Ogre::Exception &e) {
		std::cout << "Ogre Exception :" << e.what() << std::endl;
	}catch(std::exception &e) {
		std::cout << "Std Exception :" << e.what() << std::endl;
	}
	return 0;
}