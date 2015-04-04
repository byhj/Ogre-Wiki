#include <exception>
#include <Ogre/OgreRoot.h>
#include <Ogre/OgreRenderSystem.h>
#include <Ogre/OgreRenderWindow.h>
#include "SimpleOgre.h"
#include "EasyDefines.h"

  byhj::SimpleOgre::SimpleOgre(): mRoot(NULL), mWindow(NULL)
  {

  }
  byhj::SimpleOgre::~SimpleOgre()
  {
	  mWindow = NULL;
	  mRoot.reset();
  }

 bool byhj::SimpleOgre::initOgre()
  {
	  bool result = false;
	  try 
	  {
		  // step 1£ºCreate Ogre::Root object
		  Ogre::String lConfigFileName = "";
		  Ogre::String lPluginsFileName = "";
		  Ogre::String lLogFileName = "Ogre.log";
		  mRoot = std::auto_ptr<Ogre::Root>(new Ogre::Root(lConfigFileName, lPluginsFileName, lLogFileName));
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
			  mRoot->loadPlugin(lPluginName);     //load the plugin to root node
		  }

		  //step 3:create the render system
		  const Ogre::RenderSystemList & lRenderSystemList = mRoot->getAvailableRenderers();
		  if (lRenderSystemList.empty())
		  {
			  Ogre::LogManager::getSingleton().logMessage("Sorry, no rendersystem was found.");
			  return result;
		  }
		  Ogre::RenderSystem *lRenderSystem = lRenderSystemList.at(0);
		  mRoot->setRenderSystem(lRenderSystem);

		  //step 4:create a window
		  bool lCreateAWindowAutoMatically = false;
		  Ogre::String lWindowTitle = "";
		  Ogre::String lCustomCapacities = "";
		  mRoot->initialise(lCreateAWindowAutoMatically, lWindowTitle, lCustomCapacities);

		  //step 5:ask render system to create a window
		  lWindowTitle = "Hello Ogre World";
		  unsigned int lSizeX = 800;
		  unsigned int lSizeY = 600;
		  bool lFullscreen = false;
		  Ogre::NameValuePairList lParams;
		  lParams["FSAA"] = "0";
		  lParams["vsync"] = "true";
		  mWindow = mRoot->createRenderWindow(lWindowTitle, lSizeX, lSizeY, lFullscreen, &lParams);

          result = true;
		  // catch the exception
	    }catch(Ogre::Exception &e) {
		      MWARNING("Ogre::Exception!\n" << e.what() );
		      result = false;
	    }catch(std::exception &e) {
		      MWARNING( "std::exception!\n" << e.what() );
		      result = false;
	  }
	return result;
  }
