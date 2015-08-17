#include "Basic5.h"

Basic5::Basic5()
	:mRotate(.13),
	 mMove(250),
	 pmCamNode(NULL),
	 mDirection(Ogre::Vector3::ZERO)
{

}

Basic5::~Basic5()
{

}

void Basic5::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue::White);

	Ogre::Entity *pTudorEntity = mSceneMgr->createEntity("tudorhouse.mesh");
	Ogre::SceneNode *pTudorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Node");
	pTudorNode->attachObject(pTudorEntity);

	Ogre::Light *pLight = mSceneMgr->createLight("Light1");
    pLight->setType(Ogre::Light::LT_POINT);
	pLight->setPosition(Ogre::Vector3(250, 150, 250));
	pLight->setDiffuseColour(Ogre::ColourValue::White);
	pLight->setSpecularColour(Ogre::ColourValue::White);

	//Now we are going to create two SceneNodes that we can attach the Camera to
	//We will move the pointer to another SceneNode based on keyboard input.
	Ogre::SceneNode *pNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(
		"CamNode1", Ogre::Vector3(1200, -370, 0) );
	pNode->yaw(Ogre::Degree(90));
	pmCamNode = pNode;
	pNode->attachObject(mCamera);

	//create another SceneNode that we can move the Camera to.
    //pNode inherit any transformations like rotations that have been applied to the node.
	pNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CamNode2", Ogre::Vector3(-500, -370, 1000));
	pNode->yaw(Ogre::Degree(-30));

}

bool Basic5::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = Application::frameRenderingQueued(evt);
	pmCamNode->translate(mDirection * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	// the events are fed into a buffer and then dispatched via the callback methods.
	return ret;
}

bool Basic5::keyPressed(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;
    
   //Enter 1 or 2, we change the camera viewport, 
   //Use different node to attach scene camera, it will work when attach a camera
	case OIS::KC_1:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		pmCamNode = mSceneMgr->getSceneNode("CamNode1");
		pmCamNode->attachObject(mCamera);
		break;

	case OIS::KC_2:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		pmCamNode = mSceneMgr->getSceneNode("CamNode2");
		pmCamNode->attachObject(mCamera);
		break;

	case OIS::KC_UP:
	case OIS::KC_W:
		mDirection.z = -mMove;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		mDirection.z = mMove;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		mDirection.x = -mMove;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		mDirection.x = mMove;
		break;

	case OIS::KC_PGDOWN:
	case OIS::KC_E:
		mDirection.y = -mMove;
		break;

	case OIS::KC_PGUP:
	case OIS::KC_Q:
		mDirection.y = mMove;
		break;

	default:
		break;
	}

	return true;
}

bool Basic5::keyReleased(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		mDirection.z = 0;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		mDirection.z = 0;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		mDirection.x = 0;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		mDirection.x = 0;
		break;

	case OIS::KC_PGDOWN:
	case OIS::KC_E:
		mDirection.y = 0;
		break;

	case OIS::KC_PGUP:
	case OIS::KC_Q:
		mDirection.y = 0;
		break;

	default:
		break;
	}

	return true;
}

//We use mouse to change the scene view 
bool Basic5::mouseMoved(const OIS::MouseEvent& me)
{
	if (me.state.buttonDown(OIS::MB_Right))
	{
		pmCamNode->yaw(Ogre::Degree(-mRotate * me.state.X.rel), Ogre::Node::TS_WORLD);
		pmCamNode->pitch(Ogre::Degree(-mRotate * me.state.Y.rel), Ogre::Node::TS_LOCAL);
	}

	return true;
}

bool Basic5::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	Ogre::Light* light = mSceneMgr->getLight("Light1");

	switch (id)
	{
	case OIS::MB_Left:
		light->setVisible(!light->isVisible());
		break;
	default:
		break;
	}

	return true;
}

bool Basic5::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	return true;
}