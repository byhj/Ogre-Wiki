#include "Bch1.h"

BCH1::BCH1(void)
{
}

BCH1::~BCH1(void)
{

}

void BCH1::createScene()
{
	//����������ӻ�����
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	//����һ����Ⱦʵ�壬ʹ�ó�����㽫����ӵ���������ĸ������
	Ogre::Entity *ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);

	//head2
	Ogre::Entity *ogreEntity2 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * ogreNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(84, 48, 0));
	ogreNode2->attachObject(ogreEntity2);

	//head3, ������ź�λ��
	Ogre::Entity* ogreEntity3 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode3->setPosition(0, 104, 0);
	ogreNode3->setScale(2, 1.2, 1); 
	ogreNode3->attachObject(ogreEntity3);

	//head4�������ת
	Ogre::Entity* ogreEntity4 = mSceneMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode* ogreNode4 = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode4->setPosition(-84, 48, 0);
	ogreNode4->roll(Ogre::Degree(-90));  //��ʱ��Ϊ�������
	ogreNode4->attachObject(ogreEntity4);

	mCamera->setPosition(0, 47, 222);
	Ogre::Light *light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
}