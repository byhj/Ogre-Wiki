#include "ch2.h"

CH2::CH2()
{
}

CH2::~CH2()
{
}

void CH2::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);  //比较费时的一种
	Ogre::Entity *entNinja = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	entNinja->setCastShadows(true);      //阴影投射
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entNinja);
	//创建地板
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/Rockwall");    //添加材质
	entGround->setCastShadows(false);

	//点光源
	Ogre::Light *pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));
	pointLight->setDiffuseColour(1.0, 0.0, 0.0); //red color
	pointLight->setSpecularColour(1.0, 0.0, 0.0);

	//方向光
	Ogre::Light *directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	//聚光灯
	Ogre::Light *spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(300, 300, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

void CH2::createCamera()
{
	mCamera = mSceneMgr->createCamera("PlayerCam");  //创建当前场景的camera
	mCamera->setPosition(Ogre::Vector3(0, 10, 500)); //camera位置
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));   //视线方向
	mCamera->setNearClipDistance(5);  
	mCameraMan = new OgreBites::SdkCameraMan(mCamera); // create a default camera controller
}


void CH2::createViewports()
{
	Ogre::Viewport * vp = mWindow->addViewport(mCamera);  //创建camera关联的viewport
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0 ));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}