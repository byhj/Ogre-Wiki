#include "Bch2.h"

BCH2::BCH2()
{
	//null
}

BCH2::~BCH2()
{
	//null
}

void BCH2::createScene()
{
	//��ӻ����⣬���û���ͼ�� ��ӰͶ��
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);  
	Ogre::Entity *entNinja = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	entNinja->setCastShadows(true);  
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entNinja);

	//�����ذ壬Ϊ�丳�����
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *entGround = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Examples/Rockwall");    //��Ӳ���
	entGround->setCastShadows(false);

	//��ӵ��Դ
	Ogre::Light *pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));
	pointLight->setDiffuseColour(1.0, 0.0, 0.0); //red color
	pointLight->setSpecularColour(1.0, 0.0, 0.0);

	//��ӷ����
	Ogre::Light *directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.25, .25, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	//��Ӿ۹��
	Ogre::Light *spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(300, 300, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
}

void BCH2::createCamera()
{
	//Ϊ��������camera
	mCamera = mSceneMgr->createCamera("PlayerCam");  //������ǰ������camera
	mCamera->setPosition(Ogre::Vector3(0, 10, 500)); //cameraλ��
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));   //���߷���
	mCamera->setNearClipDistance(5);  
	mCameraMan = new OgreBites::SdkCameraMan(mCamera); // create a default camera controller
}

void BCH2::createViewports()
{
	//The way we tell the RenderWindow which area of the screen to use is by giving it a Viewport
	//��camera��ӵ�viewport
	Ogre::Viewport * vp = mWindow->addViewport(mCamera);  //����camera������viewport
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0 ));
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
