#include <memory>
#include <exception>
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"

#include "SimpleOgre.h"
#include "EasyDefines.h"

void run()
{
	byhj::SimpleOgre lOgre;
	if (!lOgre.initOgre())
	{
		std::cout << "Fail to init Ogre" << std::endl;
		return;
	}

	//get the scene root handle
	Ogre::Root *lRoot = lOgre.mRoot.get();
	Ogre::RenderWindow *lWindow = lOgre.mWindow;
	Ogre::SceneManager *lScene = lRoot->createSceneManager(Ogre::ST_GENERIC, "scene manager");
	Ogre::SceneNode *lRootSceneNode = lScene->getRootSceneNode();
	Ogre::Camera *lCamera = lScene->createCamera("camera");
	Ogre::SceneNode *lCameraNode = lRootSceneNode->createChildSceneNode("Camera Node");
	lCameraNode->attachObject(lCamera);

	//set the viewport
	float lViewportWidth = 0.88f;
	float lViewportHeight = 0.88f;
	float lViewportLeft	= (1.0f - lViewportWidth) * 0.5f;
	float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
	unsigned short lMainViewportZOrder = 100;
	Ogre::Viewport * vp = lWindow->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

	vp->setAutoUpdated(true);
	vp->setBackgroundColour(Ogre::ColourValue(1, 0, 1));

	float ratio = float(vp->getActualWidth()) / float(vp->getActualHeight());
	lCamera->setAspectRatio(ratio);
	lCamera->setNearClipDistance(1.5f);
	lCamera->setFarClipDistance(3000.0f); 

	lWindow->setActive(true);
	lWindow->setAutoUpdated(false);

	//create a resource group
	Ogre::String lNameOfResourceGroup = "Mission 1:Deliver Tom";
	Ogre::ResourceGroupManager &lRgMgr = Ogre::ResourceGroupManager::getSingleton();
	lRgMgr.createResourceGroup(lNameOfResourceGroup);
	lRoot->clearEventTimes();

	//add the resource
	Ogre::String lDirectoryToLoad = "../../../media/mesh";
	bool lIsRecursive = false;
	lRgMgr.addResourceLocation(lDirectoryToLoad, "FileSystem", lNameOfResourceGroup,lIsRecursive);
	lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);
	lRgMgr.loadResourceGroup(lNameOfResourceGroup);

	//add the mesh file
	Ogre::String lNameOfTheMesh = "MonsterHead.mesh";
	int lNumberOfEntities = 5;
	for (int iter = 0; iter != lNumberOfEntities; ++iter)
	{
		Ogre::Entity *lEntity = lScene->createEntity(lNameOfTheMesh);
		Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode();
		lNode->attachObject(lEntity);
		float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
		lPositionOffset = lPositionOffset * 20;
		lNode->translate(lPositionOffset, lPositionOffset, -200.0f);
	}
	Ogre::String lResourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	lRoot->clearEventTimes();
	while(!lOgre.mWindow->isClosed())  //while until close
	{
		lWindow->update(false);
		lWindow->swapBuffers();
		lRoot->renderOneFrame();

		Ogre::WindowEventUtilities::messagePump();
	}
}
int main()
{

	try
	{
		run();
		std::cout<<"end of the program"<<std::endl;
	}catch(Ogre::Exception &e)
	{
		MWARNING("!!!!Ogre::Exception!!!!\n" << e.what());
	}catch(std::exception &e)
	{
		MWARNING("!!!!std::exception!!!!\n"<<e.what());
	}
	OgreEasy::waitForUser();
}