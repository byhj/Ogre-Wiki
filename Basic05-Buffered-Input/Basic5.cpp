#include "Basic5.h"

Basic5::Basic5()
	: mRotate(.13),
	mMove(250),
	mCamNode(0),
	mDirection(Ogre::Vector3::ZERO)
{
}

Basic5::~Basic5()
{
}

void Basic5::createScene()
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.2, .2, .2));

	Ogre::Entity* tudorEntity = mSceneMgr->createEntity("tudorhouse.mesh");
	Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("TudorNode");
	node->attachObject(tudorEntity);

	Ogre::Light* light = mSceneMgr->createLight("Light1");
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(Ogre::Vector3(300, 150, 0));
	light->setDiffuseColour(Ogre::ColourValue::White);
	light->setSpecularColour(Ogre::ColourValue::White);

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CamNode1", Ogre::Vector3(1200, -370, 0));
	node->yaw(Ogre::Degree(90));

	mCamNode = node;
	node->attachObject(mCamera);

	node = mSceneMgr->getRootSceneNode()->createChildSceneNode(
		"CamNode2", Ogre::Vector3(-500, -370, 1000));
	node->yaw(Ogre::Degree(-30));

}

bool Basic5::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	bool ret = BaseApplication::frameRenderingQueued(fe);

	mCamNode->translate(mDirection * fe.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	return ret;
}

bool Basic5::keyPressed(const OIS::KeyEvent& ke)
{
	switch (ke.key)
	{
	case OIS::KC_ESCAPE:
		mShutDown = true;
		break;

	case OIS::KC_1:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		mCamNode = mSceneMgr->getSceneNode("CamNode1");
		mCamNode->attachObject(mCamera);
		break;

	case OIS::KC_2:
		mCamera->getParentSceneNode()->detachObject(mCamera);
		mCamNode = mSceneMgr->getSceneNode("CamNode2");
		mCamNode->attachObject(mCamera);
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

bool Basic5::mouseMoved(const OIS::MouseEvent& me)
{
	if (me.state.buttonDown(OIS::MB_Right))
	{
		mCamNode->yaw(Ogre::Degree(-mRotate * me.state.X.rel), Ogre::Node::TS_WORLD);
		mCamNode->pitch(Ogre::Degree(-mRotate * me.state.Y.rel), Ogre::Node::TS_LOCAL);
	}

	return true;
}

bool Basic5::mousePressed(
	const OIS::MouseEvent& me, OIS::MouseButtonID id)
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

bool Basic5::mouseReleased(
	const OIS::MouseEvent& me, OIS::MouseButtonID id)
{
	return true;
}