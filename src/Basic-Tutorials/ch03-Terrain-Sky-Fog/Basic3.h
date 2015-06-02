#ifndef BASIC3_H
#define BASIC3_H

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "OgreWiki.h"

class Basic3 :public OgreWiki::Application
{
public:
	Basic3();
	virtual ~Basic3();

protected:
	Ogre::TerrainGlobalOptions *pTerrainGlobalOptions;
	Ogre::TerrainGroup *pTerrainGroup;

	bool mTerrainsImported;

	void defineTerrain(long x, long y);
	void initBlendMaps(Ogre::Terrain *pTerrain);
	void configureTerrainDefaults(Ogre::Light *pLight);

protected:
	virtual void createScene(void);
	virtual void createFrameListener(void);
	virtual void destoryScene(void);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	//Hint the terrain is build
	OgreBites::Label *pInfoLabel;
};

#endif //Terrian