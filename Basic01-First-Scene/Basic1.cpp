#include "Basic1.h"

Basic1::Basic1(void)
{

}

Basic1::~Basic1(void)
{

}
void Basic1::createScene()
{
	//add ambient light to scene
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//the entity will be render until attach a node
	Ogre::Entity *headEntity = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode *headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(headEntity);

	//head2
	Ogre::Entity *ogreEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * ogreNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0));
	ogreNode2->attachObject(ogreEntity2);

	//head3, use scale and postion
	Ogre::Entity* ogreEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode3->setPosition(0, 104, 0);
	ogreNode3->setScale(2, 1.2, 1); 
	ogreNode3->attachObject(ogreEntity3);

	//head4£¬add rotate
	Ogre::Entity* ogreEntity4 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode4->setPosition(-84, 48, 0);
	ogreNode4->roll(Ogre::Degree(-90));  //CCW is positive
	ogreNode4->attachObject(ogreEntity4);

	//add the camera and light
	mCamera->setPosition(0, 47, 222);
	Ogre::Light *light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

}
