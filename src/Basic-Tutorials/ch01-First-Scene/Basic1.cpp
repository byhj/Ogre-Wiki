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

	//Light is not a entity, so you don't need to attch to a node for sceneMgr
	Ogre::Light *pLight = mSceneMgr->createLight("MainLight");
	pLight->setPosition(20, 80, 50);

}