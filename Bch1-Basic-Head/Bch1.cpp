#include "Bch1.h"

BCH1::BCH1(void)
{
}

BCH1::~BCH1(void)
{

}

void BCH1::createScene()
{
	//场景管理，添加环境光
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//创建一个渲染实体，使用场景结点将它添加到场景管理的跟结点上
	Ogre::Entity *ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	//head2
	Ogre::Entity *ogreEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * ogreNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0));
	ogreNode2->attachObject(ogreEntity2);

	//head3, 添加缩放和位置
	Ogre::Entity* ogreEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode3->setPosition(0, 104, 0);
	ogreNode3->setScale(2, 1.2, 1); 
	ogreNode3->attachObject(ogreEntity3);

	//head4，添加旋转
	Ogre::Entity* ogreEntity4 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode4->setPosition(-84, 48, 0);
	ogreNode4->roll(Ogre::Degree(-90));  //逆时针为正，横滚
	ogreNode4->attachObject(ogreEntity4);

	mCamera->setPosition(0, 47, 222);
	Ogre::Light *light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
}