#include "Basic2.h"

Basic2::Basic2(void)
{
}

Basic2::~Basic2(void)
{
}

void Basic2::createCamera()
{
    //Create a camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0, 10, 500) );
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void Basic2::createViewports()
{
//The way we tell the RenderWindow which area of the screen to use is by giving it a Viewport
	Ogre::Viewport *vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0) );
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth() / Ogre::Real(vp->getActualHeight()) ) );

}

void Basic2::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0) );
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	//Create a entity and attach to the scenenode(SceneMgr Create)
	Ogre::Entity *pNinjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	Ogre::SceneNode *pNinjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pNinjaEntity->setCastShadows(true);
	pNinjaNode->attachObject(pNinjaEntity);


	//Create a plane to show the shadow
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		1500, 1500, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z
		);

	Ogre::Entity *pGroundEntity = mSceneMgr->createEntity("GroudEntity", "ground");
	Ogre::SceneNode *pGroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pGroundNode->attachObject(pGroundEntity);
	pGroundEntity->setMaterialName("Examples/Rockwall");
	pGroundEntity->setCastShadows(false);

	//Add Spot Light
	Ogre::Light *pSpotLight  = mSceneMgr->createLight("SpotLight");
	pSpotLight->setDiffuseColour(0, 0, 1.0);
	pSpotLight->setDiffuseColour(0, 0, 1.0);
	pSpotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	pSpotLight->setDirection(-1, -1, 0);
	pSpotLight->setPosition(Ogre::Vector3(300, 300, 0)); 
	pSpotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50) );

	//Add point light
	Ogre::Light *pPointLight = mSceneMgr->createLight("PointLight");
	pPointLight->setType(Ogre::Light::LT_POINT);
	pPointLight->setPosition(Ogre::Vector3(0, 150, 250));
	pPointLight->setDiffuseColour(1.0, 0.0, 0.0); //red color
	pPointLight->setSpecularColour(1.0, 0.0, 0.0);

	//Add Direction light
	Ogre::Light *pDirectionalLight = mSceneMgr->createLight("DirectionalLight");
	pDirectionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	pDirectionalLight->setDiffuseColour(Ogre::ColourValue(.25, .25, 0));
	pDirectionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	pDirectionalLight->setDirection(Ogre::Vector3(0, -1, 1));
}