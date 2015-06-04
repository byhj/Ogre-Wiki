// In this program, I create a texture, in which I will render the scene.
// I create a material that uses this texture, and I apply it on a mesh.
// This technique is called a Render-To-Texture (RTT).
// Please note that depending on your video card driver, you might get very different results.
// For example, on my nvidia Geforce 9600M, RTT fails with 'FBO'.
// So I modified the SimpleOgreInit.cpp (at the 'step 3'), so that it uses "Copy" instead of FBO.

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
#include "OGRE/OgreMaterialManager.h"
#include "OGRE/OgreHardwarePixelBuffer.h"

//Here I include my other files, like the one for SimpleOgreInit...
#include "SimpleOgre.h"

#include "EasyDefines.h"

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
	unsigned short lMainViewportZOrder = 100;
	Ogre::Viewport * vp = lWindow->addViewport(lCamera, lMainViewportZOrder);

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

		// The function 'initialiseResourceGroup' parses scripts if any in the locations.
		lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);

		// Files that can be loaded are loaded.
		lRgMgr.loadResourceGroup(lNameOfResourceGroup);
	}

	// Now I create a special texture. This texture allows to do what is called a 
	// 'render to texture'. Which means that you can render your scene into 
	// it, and then use this texture as any other in a material.
	// In order to keep a good framerate, I set its number of mipmaps to 0.
	Ogre::TextureManager& lTextureManager = Ogre::TextureManager::getSingleton();
	Ogre::String lTextureName = "MyFirstRtt";
	bool lGammaCorrection = false;
	unsigned int lAntiAliasing = 0;
	unsigned int lNumMipmaps = 0;
	Ogre::TexturePtr lTextureWithRtt = lTextureManager.createManual(lTextureName, lNameOfResourceGroup, 
		Ogre::TEX_TYPE_2D, 512, 512, lNumMipmaps,
		Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET, 0, lGammaCorrection, lAntiAliasing);

	// now I will link this texture to a camera, by creating a viewport in the texture.
	Ogre::RenderTexture* lRenderTarget = NULL;
	{
		Ogre::HardwarePixelBufferSharedPtr lRttBuffer = lTextureWithRtt->getBuffer();
		lRenderTarget = lRttBuffer->getRenderTarget();
		lRenderTarget->setAutoUpdated(true);

		// I create a camera so that it has a beautiful '1' aspect ratio.
		Ogre::Camera * lRttCamera = lScene->createCamera("RttCamera");
		lRttCamera->setNearClipDistance(1.5f);
		lRttCamera->setFarClipDistance(3000.0f); 
		lRttCamera->setAspectRatio(1.0f);

		// I attach this camera to the same node than main camera.
		lCameraNode->attachObject(lRttCamera);

		// In the texture I will draw first a Big Blue Viewport.
		Ogre::Viewport* lRttViewport1 = lRenderTarget->addViewport(lRttCamera, 50, 0.00f, 0.00f, 1.0f, 1.0f);
		lRttViewport1->setAutoUpdated(true);
		Ogre::ColourValue lBgColor1(1.0,0.0,0.0,1.0);
		lRttViewport1->setBackgroundColour(lBgColor1);

		// In the texture I will draw then a more little red Viewport.
		// The previous viewport with the ZOrder at 50 will be drawn first (50 < 100).
		Ogre::Viewport* lRttViewport2 = lRenderTarget->addViewport(lRttCamera, 100, 0.05f, 0.05f, 0.9f, 0.9f);
		lRttViewport2->setAutoUpdated(true);
		Ogre::ColourValue lBgColor2(0.0,0.0,1.0,1.0);
		lRttViewport2->setBackgroundColour(lBgColor2);

		// This will draw a scene, within a scene, within a scene etc...
		// Because the quad that will be drawn is contained 'inside' the texture!
	}

	// now I create a material using this texture.
	Ogre::String lMaterialName = "MyRttMaterial";
	{
		// I get a reference on the material manager, which is a singleton.
		Ogre::MaterialManager& lMaterialManager = Ogre::MaterialManager::getSingleton();
		Ogre::MaterialPtr lMaterial = lMaterialManager.create(lMaterialName, lNameOfResourceGroup);
		Ogre::Technique * lTechnique = lMaterial->getTechnique(0);
		Ogre::Pass* lPass = lTechnique->getPass(0);
		Ogre::TextureUnitState* lTextureUnit = lPass->createTextureUnitState();
		lTextureUnit->setTextureName(lTextureName);
		//I use no mipmap, and I just use some bilinear filtering on the result.
		lTextureUnit->setNumMipmaps(0);
		lTextureUnit->setTextureFiltering(Ogre::TFO_BILINEAR);

		// I make the texture rotate.
		// The material can handle 'special effects' on the texture coordinates.
		// Here I rotate the texture coordinates.
		float lRotateSpeed = 0.01f;
		lTextureUnit->setRotateAnimation(lRotateSpeed);
		// Uncomment the following line to see something funnier. :-D.
		//lTextureUnit->setTransformAnimation(Ogre::TextureUnitState::TT_SCALE_U, Ogre::WFT_SINE, 0.9f, 0.5f, 0.0f, 0.2f);
	}

	// Now I will create a manualobject quad, and convert it to a mesh...
	Ogre::String lNameOfTheMesh = "MyQuad";
	{
		Ogre::ManualObject * lManualObject = NULL;
		Ogre::String lManualObjectName = "SomeQuad";
		lManualObject = lScene->createManualObject(lManualObjectName);

		// Always tell if you want to update the 3D (vertex/index) later or not.
		bool lDoIWantToUpdateItLater = false;
		lManualObject->setDynamic(lDoIWantToUpdateItLater);

		// BaseWhiteNoLighting is the name of a material that already exist inside Ogre.
		// Ogre::RenderOperation::OT_TRIANGLE_LIST is a kind of primitive.
		float lSize = 0.7f;
		lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		{
			float cp = 1.0f * lSize ;
			float cm = -1.0f * lSize;
			float lNumberOfTiles = 1.0f;

			lManualObject->position(cm, cp, 0.0f);// a vertex
			lManualObject->textureCoord(0.0f, 0.0f);

			lManualObject->position(cp, cp, 0.0f);// a vertex
			lManualObject->textureCoord(lNumberOfTiles, 0.0f);

			lManualObject->position(cp, cm, 0.0f);// a vertex
			lManualObject->textureCoord(lNumberOfTiles, lNumberOfTiles);

			lManualObject->position(cm, cm, 0.0f);// a vertex
			lManualObject->textureCoord(0.0, lNumberOfTiles);

			lManualObject->triangle(2,1,0);
			lManualObject->triangle(0,3,2);
		}
		lManualObject->end();
		lManualObject->convertToMesh(lNameOfTheMesh);
		lScene->destroyManualObject(lManualObject);
	}

	// Now I will create the corresponding entity, and its scenenode.
	{
		Ogre::Entity* lEntity = lScene->createEntity(lNameOfTheMesh);
		lEntity->setMaterialName(lMaterialName);
		// Now I attach it to a scenenode, so that it becomes present in the scene.
		Ogre::SceneNode* lNodeWithEntity = lNodeWithEntity = lRootSceneNode->createChildSceneNode();
		lNodeWithEntity->attachObject(lEntity);
		// I move the SceneNode so that it is visible to the camera.
		lNodeWithEntity->setPosition(0.0f, 0.0f, -1.8f);
	}

	// cleaning of windows events managed by Ogre::WindowEventUtilities::...
	// I call it after a 'pause in window updating', in order to maintain smoothness.
	// Explanation : if you clicked 2000 times when the windows was being created, there are 
	// at least 2000 messages created by the OS to listen to. This is made to clean them.
	lRoot->clearEventTimes();

	// I wait until the window is closed.
	// The "message pump" thing is something you will see in most GUI application.
	// It allow the binding of messages between the application and the OS.
	// These messages are most of the time : keystroke, mouse moved, ... or window closed.
	// If I don't do this, the message are never caught, and the window won't close.
	while(!lOgreInit.mWindow->isClosed())
	{
		// Drawings
		// the window update its content.
		// each viewport that is 'autoupdated' will be redrawn now,
		// in order given by its z-order.
		lWindow->update(false);

		// The drawn surface is then shown on the screen
		// (google "double buffering" if you want more details).
		// I always use vertical synchro.
		bool lVerticalSynchro = true;
		lWindow->swapBuffers();	

		// This update some internal counters and listeners.
		// Each render surface (window/rtt/mrt) that is 'auto-updated' has got its 'update' function called.
		lRoot->renderOneFrame();

		Ogre::WindowEventUtilities::messagePump();
	}

	// Let's cleanup!
	{
		lWindow->removeAllViewports();
	}
	{
		lScene->destroyAllCameras();
		lScene->destroyAllManualObjects();
		lScene->destroyAllEntities();
		lScene->destroyAllLights();
		lRootSceneNode->removeAndDestroyAllChildren();
	}
	{
		Ogre::RenderSystem* lRenderSystem = lRoot->getRenderSystem();
		lRenderSystem->destroyRenderTarget(lTextureName);
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

