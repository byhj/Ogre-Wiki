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

	//create the manual object

	lRoot->clearEventTimes();
	Ogre::String lManualObjectName = "Cube With Axes";
	Ogre::ManualObject *lManualObject = lScene->createManualObject(lManualObjectName);
	bool lDoIWantToUpdateItLater = false;
	lManualObject->setDynamic(lDoIWantToUpdateItLater);

	//create the cube mesh

	float lSize = 0.7f;
	lManualObject->begin("baseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	float cp = 1.0f * lSize ;
	float cm = -1.0f * lSize;
	//cube mesh point
	lManualObject->position(cm, cp, cm);// a vertex
	lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
	lManualObject->position(cp, cp, cm);// a vertex
	lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
	lManualObject->position(cp, cm, cm);// a vertex
	lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
	lManualObject->position(cm, cm, cm);// a vertex
	lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));

	lManualObject->position(cm, cp, cp);// a vertex
	lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,1.0f,1.0f));
	lManualObject->position(cp, cp, cp);// a vertex
	lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	lManualObject->position(cp, cm, cp);// a vertex
	lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,1.0f,1.0f));
	lManualObject->position(cm, cm, cp);// a vertex
	lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));

	lManualObject->triangle(0,1,2);
	lManualObject->triangle(2,3,0);
	lManualObject->triangle(4,6,5);
	lManualObject->triangle(6,4,7);

	//face top / down

	lManualObject->triangle(0,4,5);
	lManualObject->triangle(5,1,0);
	lManualObject->triangle(2,6,7);
	lManualObject->triangle(7,3,2);
	//face left / right

	lManualObject->triangle(0,7,4);
	lManualObject->triangle(7,0,3);
	lManualObject->triangle(1,5,6);
	lManualObject->triangle(6,2,1);			

	lManualObject->end();

	lManualObject->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_LIST);
	{
		float lAxeSize = 2.0f * lSize;
		lManualObject->position(0.0f, 0.0f, 0.0f);
		lManualObject->colour(Ogre::ColourValue::Red);
		lManualObject->position(lAxeSize, 0.0f, 0.0f);
		lManualObject->colour(Ogre::ColourValue::Red);
		lManualObject->position(0.0f, 0.0f, 0.0f);
		lManualObject->colour(Ogre::ColourValue::Green);
		lManualObject->position(0.0, lAxeSize, 0.0);
		lManualObject->colour(Ogre::ColourValue::Green);
		lManualObject->position(0.0f, 0.0f, 0.0f);
		lManualObject->colour(Ogre::ColourValue::Blue);
		lManualObject->position(0.0, 0.0, lAxeSize);
		lManualObject->colour(Ogre::ColourValue::Blue);

		lManualObject->index(0);
		lManualObject->index(1);
		lManualObject->index(2);
		lManualObject->index(3);
		lManualObject->index(4);
		lManualObject->index(5);
	}
	lManualObject->end();

	Ogre::String lNameOfTheMesh = "MeshCubeAndAxe";
	Ogre::String lResourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	//create the mesh and attacht the sceneNode
	lManualObject->convertToMesh(lNameOfTheMesh);
	int lNumberOfEntities = 5;
	for (int iter = 0; iter != lNumberOfEntities; ++iter)
	{
		Ogre::Entity *lEntity = lScene->createEntity(lNameOfTheMesh);
		Ogre::SceneNode *lNode = lRootSceneNode->createChildSceneNode();
		lNode->attachObject(lEntity);
		float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
		lNode->translate(lPositionOffset, lPositionOffset, -10.0f);
	}

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