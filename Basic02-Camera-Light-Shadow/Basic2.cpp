#include "Basic2.h"

Basic2::Basic2()
{
	//null
}

Basic2::~Basic2()
{
	//null
}

//use sceneMgr to add the entity and light

void Basic2::createScene()
{
	//Add the ambient light, use the shadow, attack the object to scene node
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);  
	Ogre::Entity *entNinja = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	entNinja->setCastShadows(true);  
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entNinja);

	//Create plane and add to the scene node
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/Rockwall");    //Ìí¼Ó²ÄÖÊ
	entGround->setCastShadows(false);

	//Add point light
	Ogre::Light *pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));
	pointLight->setDiffuseColour(1.0, 0.0, 0.0); //red color
	pointLight->setSpecularColour(1.0, 0.0, 0.0);

	//Add Direction light
	Ogre::Light *directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	//Add Spot light
	Ogre::Light *spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(300, 300, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

void Basic2::createCamera()
{
	//we create a camera for the scene
	mCamera = mSceneMgr->createCamera("PlayerCam"); 
	mCamera->setPosition(Ogre::Vector3(0, 10, 500)); 
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));   
	mCamera->setNearClipDistance(5);  
	//camera icon
	mCameraMan = new OgreBites::SdkCameraMan(mCamera); // create a default camera controller
}

void Basic2::createViewports()
{
	//The way we tell the RenderWindow which area of the screen to use is by giving it a Viewport
	Ogre::Viewport * vp = mWindow->addViewport(mCamera);  
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0 ));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}