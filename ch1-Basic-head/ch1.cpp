#include "ch1.h"

CH1::CH1()
{
}

CH1::~CH1()
{
}

void CH1::createScene()
{
	//mSceneMgr是默认的场景管理类，我们向其中添加对象
     mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
    //实体是对象的一种，我们从文件中加载mesh
     Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
	 //为了把实例添加到场景，我们在根节点下创建子节点，然后把对象添加到新建当前节点
     Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
     headNode->attachObject(ogreHead);
    //航向，y轴旋转
     headNode->yaw( Ogre::Degree( -90 ) );
 
     Ogre::Entity* ogreHead2 = mSceneMgr->createEntity( "Head2", "ogrehead.mesh" );
     Ogre::SceneNode* headNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "HeadNode2", Ogre::Vector3( 100, 0, 0 ) );
     headNode2->attachObject( ogreHead2 );
    //俯仰，绕x轴旋转
     headNode2->pitch( Ogre::Degree( -90 ) );
 
     Ogre::Entity* ogreHead3 = mSceneMgr->createEntity( "Head3", "ogrehead.mesh" );
     Ogre::SceneNode* headNode3 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "HeadNode3", Ogre::Vector3( 200, 0, 0 ) );
     headNode3->attachObject( ogreHead3 );
     //横滚，z轴旋转
     headNode3->roll( Ogre::Degree( -90 ) );

}