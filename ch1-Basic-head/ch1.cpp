#include "ch1.h"

CH1::CH1()
{
}

CH1::~CH1()
{
}

void CH1::createScene()
{
	//mSceneMgr��Ĭ�ϵĳ��������࣬������������Ӷ���
     mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
    //ʵ���Ƕ����һ�֣����Ǵ��ļ��м���mesh
     Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
	 //Ϊ�˰�ʵ����ӵ������������ڸ��ڵ��´����ӽڵ㣬Ȼ��Ѷ�����ӵ��½���ǰ�ڵ�
     Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
     headNode->attachObject(ogreHead);
    //����y����ת
     headNode->yaw( Ogre::Degree( -90 ) );
 
     Ogre::Entity* ogreHead2 = mSceneMgr->createEntity( "Head2", "ogrehead.mesh" );
     Ogre::SceneNode* headNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "HeadNode2", Ogre::Vector3( 100, 0, 0 ) );
     headNode2->attachObject( ogreHead2 );
    //��������x����ת
     headNode2->pitch( Ogre::Degree( -90 ) );
 
     Ogre::Entity* ogreHead3 = mSceneMgr->createEntity( "Head3", "ogrehead.mesh" );
     Ogre::SceneNode* headNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "HeadNode3", Ogre::Vector3( 200, 0, 0 ) );
     headNode3->attachObject( ogreHead3 );
     //�����z����ת
     headNode3->roll( Ogre::Degree( -90 ) );

}