#ifndef MCH6_H
#define MCH6_H

#include "BaseApplication.h"

class Mch6 : public BaseApplication
{
public:
    Mch6(void);
	~Mch6(void);
protected:
	virtual void createScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent & evt);
	virtual void createProjector();
	virtual void makeMaterialReceiveDecal(const Ogre::String &matName);

	Ogre::SceneNode *mProjetorNode;
	Ogre::Frustum *mDecalFrustum;
	Ogre::Frustum *mFilterFrustum;
	float mAnim;
};
#endif
