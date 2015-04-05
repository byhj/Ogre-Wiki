#ifndef CH3_H
#define CH3_H

#include "BaseApplication.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
class CH3 : public BaseApplication {
public:
	CH3(void);
	virtual ~CH3(void);
private:
	Ogre::TerrainGlobalOptions *mTerrainGlobals;
	Ogre::TerrainGroup *mTerrainGroup;
	bool mTerrainsImported;
	OgreBites::Label* mInfoLabel;
	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain * terrain);
	void configureTerrainDefaults(Ogre::Light *light);
protected:
    virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destroyScene();
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
};

#endif