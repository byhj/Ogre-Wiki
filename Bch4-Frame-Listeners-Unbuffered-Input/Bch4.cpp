#include "Bch4.h"

bool BCH4::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
	static bool mMouseDown = false; 
	static Ogre::Real mToggle = 0.0;
	static Ogre::Real mRotate = 0.13;
	static Ogre::Real mMove = 250;

	bool currMouse = mMouse->getMouseState().buttonDown(OIS::MB_Left);

	//左鼠标按下，关闭点光源
	if (currMouse && !mMouseDown) 
	{
		Ogre::Light *light = mSceneMgr->getLight("PointLight");  
		light->setVisible(!light->isVisible());
	}

	mMouseDown = currMouse;
	mToggle -= evt.timeSinceLastFrame;
	if ((mToggle < 0.0f) && mKeyboard->isKeyDown(OIS::KC_1))
	{
		mToggle = 0.5;
		Ogre::Light *light = mSceneMgr->getLight("PointLight");
		light->setVisible(!light->isVisible());
	}

	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;  //其它按键使得camera移动
	if (mKeyboard->isKeyDown(OIS::KC_I))
	{
		transVector.z -= mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_K)) // Backward
	{
		transVector.z += mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_J)) // Left - yaw or strafe
	{
		if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
		{
			// Yaw left
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(mRotate * 5));
		} else {
			transVector.x -= mMove; // Strafe left
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_L)) // Right - yaw or strafe
	{
		if(mKeyboard->isKeyDown( OIS::KC_LSHIFT ))
		{
			// Yaw right
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(-mRotate * 5));
		} else {
			transVector.x += mMove; // Strafe right
		}
	}
	if (mKeyboard->isKeyDown(OIS::KC_U)) // Up
	{
		transVector.y += mMove;
	}
	if (mKeyboard->isKeyDown(OIS::KC_O)) // Down
	{
		transVector.y -= mMove;
	}

	mSceneMgr->getSceneNode("NinjaNode")->translate(transVector * evt.timeSinceLastFrame,
		Ogre::Node::TS_LOCAL);
	return true;
}

bool BCH4::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	bool ret = BaseApplication::frameRenderingQueued(evt);
	if (!processUnbufferedInput(evt))
		return false;
	return ret;
}

void BCH4::createScene(void)
{
	//add the node to scene
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.25, 0.25, 0.25));
	Ogre::Entity  *ninjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
	node->attachObject(ninjaEntity);

	//set the point light
	Ogre::Light *pointLight = mSceneMgr->createLight("PointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(250, 150, 250));
	pointLight->setDiffuseColour(Ogre::ColourValue::White);
	pointLight->setSpecularColour(Ogre::ColourValue::White);
}
