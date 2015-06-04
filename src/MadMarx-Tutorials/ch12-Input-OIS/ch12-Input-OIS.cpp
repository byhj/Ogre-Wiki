// Since input is quite useful for some tests, I will show
// a very basic usage of the OIS library. Bear in mind that 
// I could have used another input system or library 
// (ex: CImg, camera tracking, conio, ...).
//
// In this program I create 5 entities, and then I will control in
// realtime the camera with the mouse, and the keyboard (T,G,F,H).
// When the left button of the mouse is pushed, the colour of the 
// viewport changes randomly.
// The escape button of the keyboard allows to quit.
// 
// The controls are made in realtime. To do that, I use Ogre's timer
// to determinate how much time as flown between 2 frames :
// speed * elapsed time between 2 frames = deplacement to do between 2 frames.
//
// I also print some informations about performances.
// Please note that you can also find all of this in the official Ogre3D tutorials.

// I will use std::auto_ptr so I need to include 'memory'. 
// If you don't know std::auto_ptr, you should check some C++ tutorials/lesson on this matter.
#include <memory>
// I will check for std::exception. If you don't know what exception/try/catch means, you should learn C++ first.
#include <exception>

// These are some files that we need to include to use Ogre3D. Note that you can at the beginnings use directly "Ogre.h", to include lots of commonly used classes.
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"

//Here I include my other files, like the one for SimpleOgreInit...
#include "SimpleOgre.h"

#include "EasyDefines.h"

#include "OIS\Ois.h"

// I declare a function in which I will make my whole application.
// This is easy then to add more things later in that function.
// The main will call this function and take care of the global try/catch.
void AnOgreApplication()
{
	// I construct my object that will allow me to initialise Ogre easily.
	byhj::SimpleOgre lOgreInit;

	if(!lOgreInit.initOgre())
	{
		std::cout<<"Impossible to init Ogre correctly."<<std::endl;
		return;
	}

	//I prefer to be able to access my variables directly.
	Ogre::Root* lRoot = lOgreInit.mRoot.get();
	Ogre::RenderWindow* lWindow = lOgreInit.mWindow;

	// Here I create the necessary objects of OIS to manage Keyboard and Mouse.
	OIS::InputManager* lInputManager = NULL;
	OIS::Mouse* lMouse = NULL;
	OIS::Keyboard* lKeyboard = NULL;
	{
		// The input/output system needs an handle to a window as input, in a string format.
		// 1/ we need to get the "windows-API" handle
		size_t windowHandle = 0;
		lWindow->getCustomAttribute("WINDOW", &windowHandle);

		// 2/ convert it into a string
		std::string windowsHandleAsString="";
		{
			std::ostringstream windowHndStr;
			windowHndStr << windowHandle;
			windowsHandleAsString = windowHndStr.str();
		}

		// 3/ we translate it into a format accepted by OIS (the input library).
		OIS::ParamList lSpecialParameters;
		lSpecialParameters.insert(std::make_pair(std::string("WINDOW"), windowsHandleAsString));

		// 4/ we create the input/output system itself.
		lInputManager = OIS::InputManager::createInputSystem( lSpecialParameters );

		// the events can be stored or not in a buffer
		bool lBufferedKeys = false;
		bool lBufferedMouse = false;

		// creation of the keyboard-representing object
		lKeyboard = static_cast<OIS::Keyboard*>(lInputManager->createInputObject(OIS::OISKeyboard, lBufferedKeys));

		// creation of the mouse-representing object
		lMouse = static_cast<OIS::Mouse*>(lInputManager->createInputObject( OIS::OISMouse, lBufferedMouse));

		// then must tell the mouse how much it is allowed to move.
		const OIS::MouseState &infoSouris = lMouse->getMouseState();
		infoSouris.width = lWindow->getWidth();
		infoSouris.height = lWindow->getHeight();
	}

	// I create a scenemanager. This is like a 'Scene', in which I can put lights, 3d objects, etc...
	// The scenemanager contains an arborescent graph of 'SceneNodes'. To manage elements of the scene,
	// I will create SceneNodes in the SceneManager, and attach the elements to the scenenodes.
	// First parameter : I select a kind of SceneManager. This may have a huge impact on performance.
	// Depending on your scene, some are better than other. The default one does no optimization at all.
	// Second parameter : I give a name to the scenemanager.
	// Note : It is easy to have more than one scenemanager (If you got 2 different scenes for example).
	Ogre::SceneManager* lScene = lRoot->createSceneManager(Ogre::ST_GENERIC, "MyFirstSceneManager");

	// The 'root SceneNode' is the only scenenode at the beginning in the SceneManager.
	// The SceneNodes can be seen as 'transformation' containers <=> it contains scale/position/rotation
	// of the objects. There is only 1 root scenenode, and all other scenenode are 
	// its direct or indirect children.
	Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();

	// I create a camera. It represent a 'point of view' in the scene.
	Ogre::Camera* lCamera = lScene->createCamera("MyFirstCamera");

	// I attach the camera to a new SceneNode. It will be easier then to move it in the scene.
	Ogre::SceneNode* lCameraNode = lRootSceneNode->createChildSceneNode("MyFirstCameraNode");
	lCameraNode->attachObject(lCamera);

	// We create a viewport on a part of the window.
	// A viewport is the link between 1 camera and 1 drawing surface (here the window).
	// I can then call 'update();' on it to make it draw the Scene from the camera.
	// You can have several viewports on 1 window.
	// Check API for details on parameters.
	float lViewportWidth = 0.88f;
	float lViewportHeight = 0.88f;
	float lViewportLeft	= (1.0f - lViewportWidth) * 0.5f;
	float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
	unsigned short lMainViewportZOrder = 100;
	Ogre::Viewport * vp = lWindow->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

	// I want the viewport to draw the scene automatically
	// when I will call lWindow->update();
	vp->setAutoUpdated(true);

	// I choose a color for this viewport. 
	// I prefer to have a bright color, to detect holes in geometry etc...
	vp->setBackgroundColour(Ogre::ColourValue(1,0,1));

	// I choose the visual ratio of the camera. To make it looks real, I want it the same as the viewport.
	float ratio = float(vp->getActualWidth()) / float(vp->getActualHeight());
	lCamera->setAspectRatio(ratio);

	// I choose the clipping far& near planes. if far/near>2000, you can get z buffer problem.
	// eg : far/near = 10000/5 = 2000 . it's ok.
	// If (far/near)>2000 then you will likely get 'z fighting' issues.
	lCamera->setNearClipDistance(1.5f);
	lCamera->setFarClipDistance(3000.0f); 

	// I want my window to be active
	lWindow->setActive(true);

	// I want to update myself the content of the window, not automatically.
	lWindow->setAutoUpdated(false);

	// Here I choose a name for a resource group. Then I create it.
	// Often, a resourcegroup is a good way to store the data corresponding
	// to a level in a game.
	Ogre::String lNameOfResourceGroup = "Mission 1 : Deliver Tom";
	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		lRgMgr.createResourceGroup(lNameOfResourceGroup);

		// We say which directories will be loaded by this resourcegroup.
		// I can add many directories, which will be loaded in same the ORDER.
		// This ORDER is extremely important : if a material is loaded <i>after</i> a mesh using this material,
		// this mesh won't be able to find the material during its loading!
		// I advise you not to use a recursive load (which load the full directory tree). 
		// Seriously, 'recursive loading' often leads to problems.
		Ogre::String lDirectoryToLoad = "../../../media/mesh";
		bool lIsRecursive = false;
		lRgMgr.addResourceLocation(lDirectoryToLoad, "FileSystem", lNameOfResourceGroup, lIsRecursive);

		// The function 'initialiseResourceGroup' parses scripts if any in the locations.
		lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);

		// Files that can be loaded are loaded.
		lRgMgr.loadResourceGroup(lNameOfResourceGroup);

		// Now the loaded Mesh is available from its ResourceGroup,
		// as well as from the Ogre::MeshManager. A shared pointer to
		// it can be accessed by : Ogre::MeshManager::getSingleton().getByName(name_of_the_mesh);

		// Now I can create Entities using that mesh.
		Ogre::String lNameOfTheMesh = "MonsterHead.mesh";
		int lNumberOfEntities = 5;
		for(int iter = 0; iter < lNumberOfEntities; ++iter)
		{
			Ogre::Entity* lEntity = lScene->createEntity(lNameOfTheMesh);
			// Now I attach it to a scenenode, so that it becomes present in the scene.
			Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode();
			lNode->attachObject(lEntity);
			// I move the SceneNode so that it is visible to the camera.
			float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
			lPositionOffset = lPositionOffset * 20;
			lNode->translate(lPositionOffset, lPositionOffset, -200.0f);
			// The loaded mesh will be white. This is normal.
		}
	}

	// cleaning of windows events managed by Ogre::WindowEventUtilities::...
	// I call it after a 'pause in window updating', in order to maintain smoothness.
	// Explanation : if you clicked 2000 times when the windows was being created, there are 
	// at least 2000 messages created by the OS to listen to. This is made to clean them.
	lRoot->clearEventTimes();

	// The root has an access to Ogre3D's internal timer.
	// This timer can be controlled (for example, if you want to synchronise
	// some shaders between 5 computers), and accessed for time informations.
	// Here I reset the timer.
	Ogre::Timer* lOgreTimer = lRoot->getTimer();
	lOgreTimer->reset();

	// I name my variable with something that indicates milliseconds or seconds.
	// Here the _Ms means 'milliseconds',and '_s' means 'seconds'.
	unsigned long lLastTime_Ms = lOgreTimer->getMilliseconds();
	unsigned long lDeltaTime_Ms = 0;
	float lDeltaTime_s = 0.0f;
	// I wait until the window is closed.
	// The "message pump" thing is something you will see in most GUI application.
	// It allow the binding of messages between the application and the OS.
	// These messages are most of the time : keystroke, mouse moved, ... or window closed.
	// If I don't do this, the message are never caught, and the window won't close.
	while(!lOgreInit.mWindow->isClosed())
	{
		// For the window drawing, you will increase performances if you : 
		// 0/ do some cpu calculations (ex: update sound).
		// 1/ clear the window buffer (Ogre::Viewport::clear)
		// 2/ do some cpu calculations (ex: particles)
		// 3/ draw the scene without swapping buffers (render() / update())
		// 4/ do some cpu calculations (ex : physics + logics)
		// 5/ swap the buffers(). (swapBuffers())
		// 6/ go back to 0/
		// Ogre allows to do that automatically if you use a framelistener,
		// like in the official ogre tutorials.
		// 
		// In this tutorial series, I don't want to bother you with that,
		// but you might keep that in mind when you design your final application.
		// Also it's better to launch sound first, because sound is slower than light.

		// I evaluate the current time and the time elapsed since last frame
		// I also prepare the next iteration.
		{
			unsigned long lCurrentTime_Ms = lOgreTimer->getMilliseconds();
			lDeltaTime_Ms = lCurrentTime_Ms - lLastTime_Ms;
			if(lDeltaTime_Ms == 0)
			{
				continue;
			}
			lLastTime_Ms = lCurrentTime_Ms;
			lDeltaTime_s = 0.001f * float(lDeltaTime_Ms);
		}

		// I capture the keyboard settings.
		// Then I update the scene according to these informations.
		lKeyboard->capture();

		// The current time is used in the calculation : this is 'real time'. 
		// The camera move with the same speed on any computer.
		// I put a coefficient 200.0 because the scene is big.
		// I test the keys TGFH for moving.
		{
			float lCoeff = 200.0f * lDeltaTime_s;
			Ogre::Vector3 lTranslation(Ogre::Vector3::ZERO);
			if(lKeyboard->isKeyDown(OIS::KC_T))
			{
				lTranslation.z -= lCoeff;
			}
			if(lKeyboard->isKeyDown(OIS::KC_G))
			{
				lTranslation.z += lCoeff;
			}
			if(lKeyboard->isKeyDown(OIS::KC_F))
			{
				lTranslation.x -= lCoeff;
			}
			if(lKeyboard->isKeyDown(OIS::KC_H))
			{
				lTranslation.x += lCoeff;
			}
			if(lTranslation != Ogre::Vector3::ZERO)
			{
				lCameraNode->translate(lTranslation, Ogre::Node::TS_LOCAL);
			}
			if(lKeyboard->isKeyDown(OIS::KC_ESCAPE))
			{
				break;
			}
		}

		// same for the mouse.
		{
			lMouse->capture();
			const OIS::MouseState& lMouseState = lMouse->getMouseState();
			if(lMouseState.buttonDown(OIS::MB_Left))
			{
				// I change the colour of the background...
				float red	= Ogre::Math::RangeRandom(0.1f,0.9f);
				float green = Ogre::Math::RangeRandom(0.1f,0.9f);
				float blue	= Ogre::Math::RangeRandom(0.1f,0.9f);
				vp->setBackgroundColour(Ogre::ColourValue( red, green, blue));
			}
			float lMouseX = float(lMouseState.X.rel) / float(lWindow->getWidth());
			float lMouseY = float(lMouseState.Y.rel) / float(lWindow->getHeight());
			float lRotCoeff = -5.0f;
			Ogre::Radian lAngleX(lMouseX * lRotCoeff);
			Ogre::Radian lAngleY(lMouseY * lRotCoeff);
			// If the 'player' don't make loopings, 'yaw in world' + 'pitch in local' is often enough for a camera controler.
			lCameraNode->yaw(lAngleX, Ogre::Node::TS_WORLD);
			lCameraNode->pitch(lAngleY, Ogre::Node::TS_LOCAL);
		}

		// the window update its content.
		// each viewport that is 'autoupdated' will be redrawn now,
		// in order given by its z-order.
		lWindow->update(false);

		// The drawn surface is then shown on the screen
		// (google "double buffering" if you want more details).
		// I always use vertical synchro.
		bool lVerticalSynchro = true;
		lWindow->swapBuffers();

		// I print some statistics (more are available in Ogre3D)
		// With VSync enabled, that should be close to 59,9fps.
		const Ogre::RenderTarget::FrameStats& lStats = lWindow->getStatistics();
		std::cout<<"FPS: "<<lStats.lastFPS<<"; AvgFPS : "<<lStats.avgFPS;
		std::cout<<"; batchcount :"<<lStats.batchCount<<std::endl;

		// This update some internal counters and listeners.
		// Each render surface (window/rtt/mrt) that is 'auto-updated' has got its 'update' function called.
		lRoot->renderOneFrame();

		Ogre::WindowEventUtilities::messagePump();
	}

	// Let's cleanup!
	{
		lInputManager->destroyInputObject(lKeyboard);
		lKeyboard = NULL;
		lInputManager->destroyInputObject(lMouse);
		lMouse = NULL;
		OIS::InputManager::destroyInputSystem(lInputManager);
		lInputManager = NULL;
	}
	{
		lWindow->removeAllViewports();
	}
	{
		lScene->destroyAllCameras();
		lScene->destroyAllManualObjects();
		lScene->destroyAllEntities();
		lRootSceneNode->removeAndDestroyAllChildren();
	}
	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		lRgMgr.destroyResourceGroup(lNameOfResourceGroup);
	}

	return;
}

int main()
{
	try
	{
		AnOgreApplication();
		std::cout<<"end of the program"<<std::endl;
	}catch(Ogre::Exception &e)
	{
		MWARNING("!!!!Ogre::Exception!!!!\n"<<e.what());
	}catch(std::exception &e)
	{
		MWARNING("!!!!std::exception!!!!\n"<<e.what());
	}
	return 0;
}

