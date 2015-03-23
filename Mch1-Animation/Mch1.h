#ifndef MCH1_H
#define MCH1_H

#include "BaseApplication.h"
#include <deque>

class Mch1: public BaseApplication {
public:
	Mch1(void);
	virtual ~Mch1(void);
protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual bool nextLocation(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

	Ogre::Real mDistance;
	Ogre::Vector3 mDirection;
	Ogre::Vector3 mDestination;

	Ogre::AnimationState *mAnimationState;
	Ogre::Entity *mEntity;
	Ogre::SceneNode *mNode;

	Ogre::Real mWalkSpeed; 
	std::deque<Ogre::Vector3> mWalkList;
};

#endif
