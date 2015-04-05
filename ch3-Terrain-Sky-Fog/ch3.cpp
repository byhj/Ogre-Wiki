#include "ch3.h"

CH3::CH3(void)
{
}

CH3::~CH3(void)
{
}

void CH3::destroyScene(void)
{
	OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}



void getTerrainImage(bool flipX, bool flipY, Ogre::Image &img)
{
	    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    if (flipX)
        img.flipAroundY();  //使用纹理填充
    if (flipY)
        img.flipAroundX();
}

void CH3::defineTerrain(long x, long y)
{
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
		mTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage( x %2 != 0,y %2 !=0, img);
			mTerrainGroup->defineTerrain(x, y, &img);
		    mTerrainsImported = true;
	}

}

void CH3::initBlendMaps(Ogre::Terrain *terrain)
{

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void CH3::configureTerrainDefaults(Ogre::Light *light)
{
	//地形的选项设置
	mTerrainGlobals->setMaxPixelError(8);
	mTerrainGlobals->setCompositeMapDistance(3000);
	mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
	//地形的参数设置
	Ogre::Terrain::ImportData &defaultImp = mTerrainGroup->getDefaultImportSettings();
	defaultImp.terrainSize = 513;
	defaultImp.worldSize = 12000.0f;
	defaultImp.inputScale = 600;
	defaultImp.minBatchSize = 33;
	defaultImp.maxBatchSize = 65;
	//texture
	//diffuse_specular - diffuse texture with a specular map in the alpha channel
    //normal_height - normal map with a height map in the alpha channel
	//层次渲染
	defaultImp.layerList.resize(3);
	defaultImp.layerList[0].worldSize = 100;
	defaultImp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultImp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultImp.layerList[1].worldSize = 30;
	defaultImp.layerList[1].textureNames.push_back("grass_green-01-diffusespecular.dds");
	defaultImp.layerList[1].textureNames.push_back("grass_green_01_normalheight.dds");
	defaultImp.layerList[2].worldSize = 200;
	defaultImp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	defaultImp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void CH3::createScene(void)
{
	//创建camera
	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
	mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
	mCamera->setNearClipDistance(0.1);
	//添加light
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE));
	   mCamera->setFarClipDistance(0);

	   Ogre::Vector3 lightDir(0.55, -0.37, 0.75);
	   lightDir.normalise();
	   Ogre::Light *light = mSceneMgr->createLight("TstLight");
	   light->setType(Ogre::Light::LT_DIRECTIONAL);
	   light->setDirection(lightDir);
	   light->setDiffuseColour(Ogre::ColourValue::White);
	   light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
	   
	   mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
   //terrain options
	   mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	   mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513,12000.0f);
	   mTerrainGroup->setFilenameConvention(Ogre::String("CH3Terrian"),
		   Ogre::String("dat"));
	   mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
	   configureTerrainDefaults(light);

	   for (long x = 0; x <= 0; ++x)
		   for (long y = 0; y<=0; ++y)
			   defineTerrain(x, y);
	    
    // sync load since we want everything in place when we start
	   mTerrainGroup->loadAllTerrains(true);
	   if (mTerrainsImported)
	   {
		   Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		   while(ti.hasMoreElements()) 
		   {
			   Ogre::Terrain *t = ti.getNext()->instance;
			   initBlendMaps(t);
		   }
	   }
	   mTerrainGroup->freeTemporaryResources();
       Ogre::ColourValue fadeColour(0.1, 0.1, 0.1);
       mWindow->getViewport(0)->setBackgroundColour(fadeColour);
       mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 150);
 
       Ogre::Plane plane;
       plane.d = 10;
       plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
 
       mSceneMgr->setSkyPlane(true, plane, "Examples/SpaceSkyPlane", 100, 45, true, 0.5, 150, 150);
}

void CH3::createFrameListener(void)
{
	 BaseApplication::createFrameListener();
     mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TInfo", "", 350);
}

bool CH3::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    bool ret = BaseApplication::frameRenderingQueued(evt);
 
    if (mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
        mInfoLabel->show();
        if (mTerrainsImported)
        {
            mInfoLabel->setCaption("Building terrain, please wait...");
        }
        else
        {
            mInfoLabel->setCaption("Updating textures, patience...");
        }
    }
    else
    {
        mTrayMgr->removeWidgetFromTray(mInfoLabel);
        mInfoLabel->hide();
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }
 
    return ret;
}
