#include "Basic3.h"

Basic3::Basic3(void)
	:pTerrainGlobalOptions(0), pTerrainGroup(0), pInfoLabel(0)
{

}

Basic3::~Basic3(void)
{

}

void Basic3::destoryScene(void)
{
	OGRE_DELETE pTerrainGroup;
	OGRE_DELETE pTerrainGlobalOptions;
}

void Basic3::createScene(void)
{
	//set the camera
	mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
	mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
	mCamera->setNearClipDistance(0.1);

	//check if support the infinite clip 
	bool infiniteClip = mRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE);
	if (infiniteClip)
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(500000);

	//Set the light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	Ogre::Vector3 lightDir(0.55, -0.3, 0.75);
	lightDir.normalise();
	Ogre::Light *pLight = mSceneMgr->createLight("TestLight");
	pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	pLight->setDirection(lightDir);
	pLight->setDiffuseColour(Ogre::ColourValue::White);
	pLight->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

	//Configuring the Terrain
	pTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions;
	pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0);
	pTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	//Use Light and define the terrain
	configureTerrainDefaults(pLight);
	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);
	pTerrainGroup->loadAllTerrains(true);


	//After imported, we set the blend map
	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator iter = pTerrainGroup->getTerrainIterator();
		while (iter.hasMoreElements())
		{
			Ogre::Terrain* t = iter.getNext()->instance;
			initBlendMaps(t);
		}
	}
	//remeber to free resources after load
	pTerrainGroup->freeTemporaryResources();

	//add fog effect
	Ogre::ColourValue fadeColour(0.1, 0.1, 0.1);
	mWindow->getViewport(0)->setBackgroundColour(fadeColour);
	mSceneMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 150);

	//add skybox
	Ogre::Plane plane;
	plane.d = 10;
	plane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	mSceneMgr->setSkyPlane(true, plane, "Examples/SpaceSkyPlane", 100, 45, true, 0.5, 150, 150);
}

void Basic3::createFrameListener()
{
	Application::createFrameListener();
	pInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TerrainInfo", "", 350);
}

bool Basic3::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ret = Application::frameRenderingQueued(evt);

	//we use the label to hint terrian if load
	if (pTerrainGroup->isDerivedDataUpdateInProgress())
	{
		mTrayMgr->moveWidgetToTray(pInfoLabel, OgreBites::TL_TOP, 0);
		pInfoLabel->show();

		if (mTerrainsImported)
			pInfoLabel->setCaption("Building terrain...");
		else   
			pInfoLabel->setCaption("Updating terrain...");
	}
	else
	{
		mTrayMgr->removeWidgetFromTray(pInfoLabel);
		pInfoLabel->hide();

		if (mTerrainsImported)
		{
			pTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}
	}
	return ret;
}

void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

void Basic3::defineTerrain(long x, long y)
{
	Ogre::String filename = pTerrainGroup->generateFilename(x, y);
	bool exists =
		Ogre::ResourceGroupManager::getSingleton().resourceExists(
		pTerrainGroup->getResourceGroup(),
		filename);
	if (exists)
		pTerrainGroup->defineTerrain(x, y);
	else
	{
		Ogre::Image img;
		getTerrainImage(x % 2 != 0, y % 2 != 0, img);
		pTerrainGroup->defineTerrain(x, y, &img);

		mTerrainsImported = true;
	}
}

//blend together the different layers texture we defined in configureTerrainDefaults
void Basic3::initBlendMaps(Ogre::Terrain* terrain)
{

	Ogre::Real minHeight0 = 70;
	Ogre::Real fadeDist0 = 40;
	Ogre::Real minHeight1 = 70;
	Ogre::Real fadeDist1 = 15;

	Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
	Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

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

void Basic3::configureTerrainDefaults(Ogre::Light* light)
{
	//Terrian global setting
	pTerrainGlobalOptions->setMaxPixelError(8);
	pTerrainGlobalOptions->setCompositeMapDistance(3000);
	pTerrainGlobalOptions->setLightMapDirection(light->getDerivedDirection());
	pTerrainGlobalOptions->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
	pTerrainGlobalOptions->setCompositeMapDiffuse(light->getDiffuseColour());

	//Import Setting
	Ogre::Terrain::ImportData& importData = pTerrainGroup->getDefaultImportSettings();
	importData.terrainSize = 513;
	importData.worldSize = 12000.0;
	importData.inputScale = 600;
	importData.minBatchSize = 33;
	importData.maxBatchSize = 65;
	importData.layerList.resize(3);

	//Add the layer texture
	importData.layerList[0].worldSize = 100;
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	importData.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
	
	importData.layerList[1].worldSize = 30;
	importData.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
	importData.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
	
	importData.layerList[2].worldSize = 200;
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
	importData.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");

}