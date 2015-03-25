#include "Mch5.h"

Mch5::Mch5(void)
{

}

Mch5::~Mch5(void)
{

}

void Mch5::createGrassMesh()
{
	const float width = 25;
	const float height = 30;
	Ogre::ManualObject mo("GrassObject");

	Ogre::Vector3 vec(width / 2, 0, 0);
	Ogre::Quaternion rot;
	rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);
	mo.begin("Example/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (int i = 0; i != 3; ++i)
	{
		mo.position(-vec.x, height, -vec.z);
		mo.textureCoord(0, 0);

		mo.position(vec.x, height, vec.z);
		mo.textureCoord(1, 0);

		mo.position(-vec.x, 0, -vec.z);
		mo.textureCoord(0, 1);

		mo.position(vec.x, 0, vec.z);
		mo.textureCoord(1, 1);
		int offset = i * 4;
		mo.triangle(offset, offset+3, offset+1);
		mo.triangle(offset, offset+2, offset+3);
		vec = rot * vec;
	}
	mo.end();
	mo.convertToMesh("GrassBladesMesh");
	Ogre::Entity *grass = mSceneMgr->createEntity("grass", "GrassBladesMesh");
	Ogre::StaticGeometry *sg = mSceneMgr->createStaticGeometry("GrassArea");

	const int size = 375;
	const int amount = 20;
	sg->setRegionDimensions(Ogre::Vector3(size, size, size));
	sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

	for (int x = -size/2; x < size/2; x += (size/amount))
	{
		for (int z = -size/2; z < size/2; z += (size/amount))
		{
			Ogre::Real r = size / (float)amount / 2;
			Ogre::Vector3 pos(x + Ogre::Math::RangeRandom(-r, r), 0, z + Ogre::Math::RangeRandom(-r, r));
			Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);
			Ogre::Quaternion orientation;
			orientation.FromAngleAxis(Ogre::Degree(Ogre::Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);

			sg->addEntity(grass, pos, orientation, scale);
		}
	}
	sg->build();
}

void Mch5::createScene()
{
   createGrassMesh();
   //light
   mSceneMgr->setAmbientLight(Ogre::ColourValue::White);
   mCamera->setPosition(150, 50, 150);
   mCamera->lookAt(0, 0, 0);

   Ogre::Entity *robot = mSceneMgr->createEntity("robot", "robot.mesh");
   mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(robot);

   Ogre::Plane plane;
   plane.normal = Ogre::Vector3::UNIT_Y;
   plane.d = 0;

   Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	   plane, 450.0f, 450.0f, 10, 10, true, 1, 50.0f, 50.0f, Ogre::Vector3::UNIT_Z);

   Ogre::Entity *planeEnt = mSceneMgr->createEntity("plane", "floor");
   planeEnt->setMaterialName("Examples/GrassFloor");
   planeEnt->setCastShadows(false);
   mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEnt);
   
}

