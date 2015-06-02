#include "Basic1.h"

Basic1::Basic1(void)
{
}

Basic1::~Basic1(void)
{

}

void Basic1::createScene()
{
	mSceneMgr->setAmbientLight( Ogre::ColourValue(0.5f, 0.5f, 0.5f) );
	
	//Create a entity and attach to the scenenode(SceneMgr Create)
	Ogre::Entity *pHeadEntity = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode *pHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pHeadNode->attachObject(pHeadEntity);

	//Create another enity
	Ogre::Entity *pHeadEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode *pHeadNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0) );
	pHeadNode2->attachObject(pHeadEntity2);

	Ogre::Entity *pHeadEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode *pHeadNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//Use node to change information
	pHeadNode3->setPosition(0, 104, 0);
	pHeadNode3->setScale(2, 1.2, 1);
	pHeadNode3->attachObject(pHeadEntity3);

	Ogre::Entity *pHeadEntity4 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode *pHeadNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	pHeadNode4->setPosition(-84, 48, 0);
	pHeadNode4->roll(Ogre::Degree(-90) );
	pHeadNode4->attachObject(pHeadEntity4);
	
	//Light is not a entity, so you don't need to attch to a node for sceneMgr
	Ogre::Light *pLight = mSceneMgr->createLight("MainLight");
	pLight->setPosition(20, 80, 50);

	mCamera->setPosition(0, 47, 222);

}