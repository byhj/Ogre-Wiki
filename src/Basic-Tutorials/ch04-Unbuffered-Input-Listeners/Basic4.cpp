#include "Basic4.h"

Basic4::Basic4()
{

}

Basic4::~Basic4()
{

}

void Basic4::createScene(void)
{
    //Set the light 
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.25, .25, .25) );
	Ogre::Light *pPointLight = mSceneMgr->createLight("PointLight");
	pPointLight->setType(Ogre::Light::LT_POINT);
	pPointLight->setPosition(250, 150, 250);
	pPointLight->setDiffuseColour(Ogre::ColourValue::White);
	pPointLight->setSpecularColour(Ogre::ColourValue::White);

	//Create the node and attach entity
	Ogre::Entity *pNinjaEntity = mSceneMgr->createEntity("ninja.mesh");
	Ogre::SceneNode *pNinjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
    pNinjaNode->attachObject(pNinjaEntity);

}


bool Basic4::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	bool ret = Application::frameRenderingQueued(evt);

	//We proces unbuffered input every render frame
	if (!processUnbufferedInput(evt))
		return false;

	return ret;
}

bool Basic4::processUnbufferedInput(const Ogre::FrameEvent &evt)
{
	static bool mouseDownLastFrame = false;
	static Ogre::Real toggleTimer = 0.0;
	static Ogre::Real rotate = .13;
	static Ogre::Real move = 250;

	// frameRenderQueued has call the nouse and key capture() function in Base class 
	//So, we just query the key and mouse status;
	
    //If left mouse is down, we turn off the point light
	bool leftMouseDown = mMouse->getMouseState().buttonDown(OIS::MB_Left);
	if (leftMouseDown && !mouseDownLastFrame)
	{
		Ogre::Light *pLight = mSceneMgr->getLight("PointLight");
		pLight->setVisible(!pLight->isVisible());
	}

	//mouseDownLastFrame will be used to keep track of 
	//whether the left mouse button was held down during the last frame. 
	//If we do not do this, then the unbuffered input would turn the light on and off many times whenever we clicked. 
	//This is because the user's click will almost assuredly last longer than one frame.
	mouseDownLastFrame = leftMouseDown;

	//ing a timer that resets after either mouse button has been pressed. 
	toggleTimer -= evt.timeSinceLastFrame;
	if ((toggleTimer < 0.0f) && mKeyboard->isKeyDown(OIS::KC_1))
	{
		toggleTimer = 0.5;
		Ogre::Light *pLight = mSceneMgr->getLight("PointLight");
		pLight->setVisible(!pLight->isVisible());
	}

	//use key to control the entity
	Ogre::Vector3 dirVec = Ogre::Vector3::ZERO;
	if (mKeyboard->isKeyDown(OIS::KC_I))
		dirVec.z -= move;
	if (mKeyboard->isKeyDown(OIS::KC_K))
		dirVec.z += move;
	if (mKeyboard->isKeyDown(OIS::KC_U))
		dirVec.y += move;
	if (mKeyboard->isKeyDown(OIS::KC_O))
		dirVec.y -= move;
	if (mKeyboard->isKeyDown(OIS::KC_J))
	{    
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(5 * rotate));
		else
			dirVec.x -= move;
	}
	if (mKeyboard->isKeyDown(OIS::KC_L))
	{
		if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			mSceneMgr->getSceneNode("NinjaNode")->yaw(Ogre::Degree(-5 * rotate));
		else
			dirVec.x += move;
	}
	if (mKeyboard->isKeyDown(OIS::KC_U)) // Up
	{
		dirVec.y += move;
	}
	if (mKeyboard->isKeyDown(OIS::KC_O)) // Down
	{
		dirVec.y -= move;
	}

	mSceneMgr->getSceneNode("NinjaNode")->translate(dirVec * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	return true;
}
