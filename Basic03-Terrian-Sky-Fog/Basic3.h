#ifndef BASIC3_H
#define BASIC3_H

#include "OgreWiki.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class Basic3:public BaseApplication
{
public:
	Basic3(void);
	virtual ~Basic3(void);

protected:
	//Terrain member and function
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
	//hint the terrian is build
	OgreBites::Label* mInfoLabel;
};

#endif