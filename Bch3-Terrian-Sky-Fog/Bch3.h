#ifndef BCH3_H
#define BCH3_H

#include "OgreWiki.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class BCH3:public BaseApplication
{
public:
   BCH3(void);
   virtual ~BCH3(void);
protected:
	Ogre::TerrainGlobalOptions *mTerrainGlobals;
	Ogre::TerrainGroup *mTerrainGroup;
	bool mTerrainsImported;

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain *terrian);
	void configureTerrainDefaults(Ogre::Light *light);
protected:	
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destoryScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	OgreBites::Label* mInfoLabel;
};

#endif