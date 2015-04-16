// Same than the previous MaterialBasic (part1), but with transparency, decals,
// environment map, and mix percentage.

// I will use std::auto_ptr so I need to include 'memory'. 
// If you don't know std::auto_ptr, you should check some C++ tutorials/lesson on this matter.
#include <memory>
// I will check for std::exception. If you don't know what exception/try/catch means, you should learn C++ first.
#include <exception>

// These are some files that we need to include to use Ogre3D. Note that you can at the beginnings use directly "Ogre.h", to include lots of commonly used classes.
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreMaterialManager.h"

//Here I include my other files, like the one for SimpleOgreInit...
#include "SimpleOgre.h"

#include "EasyDefines.h"

// I declare a function in which I will make my whole application.
// This is easy then to add more things later in that function.
// The main will call this function and take care of the global try/catch.
void AnOgreApplication()
{
	// I construct my object that will allow me to initialise Ogre easily.
	byhj::SimpleOgre lOgreInit;

	if(!lOgreInit.initOgre())
	{
		std::cout<<"Impossible to init Ogre correctly."<<std::endl;
		return;
	}

	//I prefer to be able to access my variables directly.
	Ogre::Root* lRoot = lOgreInit.mRoot.get();
	Ogre::RenderWindow* lWindow = lOgreInit.mWindow;

	// I create a scenemanager. This is like a 'Scene', in which I can put lights, 3d objects, etc...
	// The scenemanager contains an arborescent graph of 'SceneNodes'. To manage elements of the scene,
	// I will create SceneNodes in the SceneManager, and attach the elements to the scenenodes.
	// First parameter : I select a kind of SceneManager. This may have a huge impact on performance.
	// Depending on your scene, some are better than other. The default one does no optimization at all.
	// Second parameter : I give a name to the scenemanager.
	// Note : It is easy to have more than one scenemanager (If you got 2 different scenes for example).
	Ogre::SceneManager* lScene = lRoot->createSceneManager(Ogre::ST_GENERIC, "MyFirstSceneManager");

	// The 'root SceneNode' is the only scenenode at the beginning in the SceneManager.
	// The SceneNodes can be seen as 'transformation' containers <=> it contains scale/position/rotation
	// of the objects. There is only 1 root scenenode, and all other scenenode are 
	// its direct or indirect children.
	Ogre::SceneNode* lRootSceneNode = lScene->getRootSceneNode();

	// I create a camera. It represent a 'point of view' in the scene.
	Ogre::Camera* lCamera = lScene->createCamera("MyFirstCamera");

	// I attach the camera to a new SceneNode. It will be easier then to move it in the scene.
	Ogre::SceneNode* lCameraNode = lRootSceneNode->createChildSceneNode("MyFirstCameraNode");
	lCameraNode->attachObject(lCamera);

	// We create a viewport on a part of the window.
	// A viewport is the link between 1 camera and 1 drawing surface (here the window).
	// I can then call 'update();' on it to make it draw the Scene from the camera.
	// You can have several viewports on 1 window.
	// Check API for details on parameters.
	float lViewportWidth = 0.88f;
	float lViewportHeight = 0.88f;
	float lViewportLeft	= (1.0f - lViewportWidth) * 0.5f;
	float lViewportTop = (1.0f - lViewportHeight) * 0.5f;
	unsigned short lMainViewportZOrder = 100;
	Ogre::Viewport * vp = lWindow->addViewport(lCamera, lMainViewportZOrder, lViewportLeft, lViewportTop, lViewportWidth, lViewportHeight);

	// I want the viewport to draw the scene automatically
	// when I will call lWindow->update();
	vp->setAutoUpdated(true);

	// I choose a color for this viewport. 
	// I prefer to have a bright color, to detect holes in geometry etc...
	vp->setBackgroundColour(Ogre::ColourValue(1,0,1));

	// I choose the visual ratio of the camera. To make it looks real, I want it the same as the viewport.
	float ratio = float(vp->getActualWidth()) / float(vp->getActualHeight());
	lCamera->setAspectRatio(ratio);

	// I choose the clipping far& near planes. if far/near>2000, you can get z buffer problem.
	// eg : far/near = 10000/5 = 2000 . it's ok.
	// If (far/near)>2000 then you will likely get 'z fighting' issues.
	lCamera->setNearClipDistance(1.5f);
	lCamera->setFarClipDistance(3000.0f); 

	// I want my window to be active
	lWindow->setActive(true);

	// I want to update myself the content of the window, not automatically.
	lWindow->setAutoUpdated(false);

	// I create a light. The scenemanager will contain it.
	// I will attach this light to a scenenode, so that I can move it easily.
	// (This will add 1 transformation matrix, but I prefer to control motion homogeneously for all elements).
	Ogre::SceneNode* lLightSceneNode = NULL;
	{
		Ogre::Light* lLight = lScene->createLight();

		// I can set some attributes of the light.
		// The basic light type can be : 
		//		pointlight (like a candle?)
		//		spotlight (kind of 'conic' light)
		//		directional light (like the sun in an outdoor scene).
		// Directional light is like parallel rays coming from 1 direction.
		lLight->setType(Ogre::Light::LT_DIRECTIONAL);

		// Here I choose the color of the light.
		// The diffuse color is the main color of the light.
		// The specular color is its color when reflected on an imperfect surface.
		// For example, when my bald head skin reflect the sun, it makes a bright round of specular color.
		//
		// The final color of an object also depends on its material.
		lLight->setDiffuseColour(1.0f, 1.0f, 1.0f);
		lLight->setSpecularColour(1.0f, 1.0f, 1.0f);// color of 'reflected' light

		lLightSceneNode = lRootSceneNode->createChildSceneNode();
		lLightSceneNode->attachObject(lLight);
	}

	// I get a reference on the material manager, which is a singleton.
	Ogre::MaterialManager& lMaterialManager = Ogre::MaterialManager::getSingleton();

	// Here I choose a name for a resource group. Then I create it.
	// Often, a resourcegroup is a good way to store the data corresponding
	// to a level in a game.
	Ogre::String lNameOfResourceGroup = "Mission 1 : Deliver Tom";
	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		lRgMgr.createResourceGroup(lNameOfResourceGroup);
		
		// We say which directories will be loaded by this resourcegroup.
		// I can add many directories, which will be loaded in same the ORDER.
		// This ORDER is extremely important : if a material is loaded <i>after</i> a mesh using this material,
		// this mesh won't be able to find the material during its loading!
		// I advise you not to use a recursive load (which load the full directory tree). 
		// Seriously, 'recursive loading' often leads to problems.
		Ogre::String lDirectoryToLoadTextures = "../../media/textures";
		bool lIsRecursive = false;
		lRgMgr.addResourceLocation(lDirectoryToLoadTextures, "FileSystem", lNameOfResourceGroup, lIsRecursive);
		
		// The function 'initialiseResourceGroup' parses scripts if any in the locations.
		lRgMgr.initialiseResourceGroup(lNameOfResourceGroup);

		// Files that can be loaded are loaded.
		lRgMgr.loadResourceGroup(lNameOfResourceGroup);

		// I/ A material allows to configure some parameters used for rendering things.
		// For example : which color, texture, shader to use.
		// Once a material has been created, it has got a name (type Ogre::String),
		// which can be used to retrieve it or apply it to an element.
		// 
		// II/ An Ogre3D material contains 1 or more 'Technique'.
		// A Material may use more than 1 Technique when it needs to use different levels of visual quality :
		// Technique : Example 1 : you want your material use 4 texture with a big shader when you are very close to the material,
		// and you want the same material to use only a grey color without shaders when the material is far away.
		// Technique : Example 2 : you want your material use 4 texture with a big shader when the program is running on high quality graphic card,
		// and you want the same material to use only a very simple color when you are on a crappy computer.
		// As a consequence, in this tutorial, I will use only 1 technique.
		// 
		// III/ A 'Technique' contains 1 or more 'Pass'.
		// A 'Pass' is 1 rendering operation. 
		// If you got 2 passes in your system, the object that use this material will be drawn 2 times (twice)
		// at the same place. If the second pass is not transparent / blended at all, it will likely overwrite the first 'Pass'.
		// You can think of a Pass like 1 'paint layer'. Yes a Layer.
		// So if you got experience with the Gimp or Photoshop, you see what I mean.
		// The 'Pass' has got several attributes, which can be very easily tweaked.
		// The most used are the following :
		// colors for realtime lighting : ambiant / diffuse / specular / shininess
		// scene blending mode : this allow to make a transparent 'Pass' (like a glass/window...).
		// alpha rejection : this allow to make decals (like foliage / trees / grass) easily.
		// depth write and depth check : manage the ZBuffer. Often necessary to tweak if you use scene blending mode.
		// color write : to allow color to be drawn or not.
		// There are several other (including shaders, depth bias, ...).
		//
		// IV/ A 'Pass' also contains 0 or more 'TextureUnit' (or 'TextureUnitState').
		// A 'TextureUnit' reference 1 'Texture' (sometimes more, when there is an animated texture or a cubic one).
		// A 'TextureUnit' has some parameters that will be used on the corresponding texture. The most used are :
		// The name of the texture (often the name of the 2D file, ex : hello.jpg)
		// The index of the texture coord (it tells which UV to choose cf Blender/3dsmax/maya).
		// The kind of environment map (none, planar, spherical, etc ...)
		// The kind of filtering and the number of mipmaps (this can have great impact on performance!).
		// There are several other attributes (for merging textures, distording them, animating them...)
		// Some that allow the use of projective texture mapping.
		//
		// Most details are in the Ogre Manual (see 'material scripts'). 
		// So you should definitely read it.
		// You should read it.
		// Read it.
		//
		// After reading the manual, you might also check the wiki, to see examples of materials.

		// Now let's see the most common beginners materials.

		// a material that uses transparency
		{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("M_Transparency",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);

			float transparency = 0.3f;
			Ogre::ColourValue lSelfIllumnationColour(0.1f, 0.0f, 0.0f, transparency);
			lFirstPass->setSelfIllumination(lSelfIllumnationColour);

			Ogre::ColourValue lDiffuseColour(1.0f, 0.4f, 0.4f, transparency);
			lFirstPass->setDiffuse(lDiffuseColour);

			Ogre::ColourValue lAmbientColour(0.4f, 0.1f, 0.1f, transparency);
			lFirstPass->setAmbient(lAmbientColour);

			// I want the specular to not be transparent.
			Ogre::ColourValue lSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
			lFirstPass->setSpecular(lSpecularColour);

			Ogre::Real lShininess = 64.0f;
			lFirstPass->setShininess(lShininess);

			// This pass will use the "alpha" to make things transparent.
			// I disallow depth write (don't write in the Z-Buffer, check an opengl book for details).
			lFirstPass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
			lFirstPass->setDepthWriteEnabled(false);

			// The corresponding script :
/*
material M_Transparency
{
	technique
	{
		pass
		{
			ambient 0.4 0.1 0.1 0.3
			diffuse 1.0 0.4 0.4 0.3
			specular 1.0 1.0 1.0 1.0 64.0
			emissive 0.1 0.0 0.0 0.3
			scene_blend alpha_blend
			depth_write off
		}
	}
}
*/
		}
		
		// A material that uses a decal (foliage, grass, ... ) : transparency 'one or zero' based on a texture.
		// This is very different of previous technique : some pixels are not even drawn!
		// You also don't need to stop ZBuffer write, as a consequence, it's perfect for forests.
		{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("M_Decals",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			lFirstPass->setLightingEnabled(false);

			unsigned char alpha_threshold = 120;
			bool useAlphaToCoverage = false;
			lFirstPass->setAlphaRejectSettings(Ogre::CMPF_GREATER, alpha_threshold, useAlphaToCoverage);

			Ogre::TextureUnitState* lTextureUnit = lFirstPass->createTextureUnitState();
			lTextureUnit->setTextureName("gras_02.png", Ogre::TEX_TYPE_2D);
			lTextureUnit->setTextureCoordSet(1);
			// The corresponding script :
/*
material M_Decals
{
	technique
	{
		pass
		{
			lighting off
			alpha_rejection greater 120
			texture_unit
			{
				texture gras_02.png 2d
				tex_coord_set 1
			}
		}
	}
}
*/
		}

		// a material that uses an environment map. The texture will move according to the viewer 's position.
		// This gives the feeling that something is reflected.
		// Perfect for cars, metals, liquids. Use it with other textures simultaneously.
		{
			Ogre::MaterialPtr lMaterial = lMaterialManager.create("M_EnvMap",lNameOfResourceGroup);
			Ogre::Technique* lFirstTechnique = lMaterial->getTechnique(0);
			Ogre::Pass* lFirstPass = lFirstTechnique->getPass(0);
			
			Ogre::TextureUnitState* lTextureUnit = lFirstPass->createTextureUnitState();
			lTextureUnit->setTextureName("SimpleTexture.bmp", Ogre::TEX_TYPE_2D);
			
			// I want this texture to simulate a planar reflection.
			// For curved / spherical object, you'd better use the other Ogre::TextureUnitState::ENV_...
			lTextureUnit->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_PLANAR);
			
			// The corresponding script :
/*
material M_EnvMap
{
	technique
	{
		pass
		{
			lighting off
			texture_unit
			{
				texture SimpleTexture.bmp 2d
				env_map planar
			}
		}
	}
}
*/
		}

		// No lighting + 1 texture with percentage influence + 1 texture.
		// Useful if you want to have a [diffuse + 'percentage of lightmap'].
		{
			float lPercentageSecondTexture = 0.2f;

			Ogre::MaterialPtr lMaterial = lMaterialManager.create("M_NoLighting+PercentageOnlyOnOneTexture",lNameOfResourceGroup);
			Ogre::Material::TechniqueIterator lIterTechnique = lMaterial->getTechniqueIterator();
			Ogre::Technique* lFirstTechnique = lIterTechnique.getNext();
			Ogre::Technique::PassIterator lIterPass = lFirstTechnique->getPassIterator();
			Ogre::Pass* lFirstPass = lIterPass.getNext();
			lFirstPass->setLightingEnabled(false);

			Ogre::TextureUnitState* lTextureUnit2 = lFirstPass->createTextureUnitState();
			lTextureUnit2->setTextureName("SimpleTexture2.bmp", Ogre::TEX_TYPE_2D);
			lTextureUnit2->setTextureCoordSet(1);
			// I say that I want to blend manually between the selected texture, and everything that has already been drawn.
			lTextureUnit2->setColourOperationEx(Ogre::LBX_BLEND_MANUAL, Ogre::LBS_TEXTURE,
				Ogre::LBS_CURRENT, Ogre::ColourValue::White, Ogre::ColourValue::White, lPercentageSecondTexture); 

			
			// This texture will be multiplied normally.
			Ogre::TextureUnitState* lTextureUnit = lFirstPass->createTextureUnitState();
			lTextureUnit->setTextureName("SimpleTexture.bmp", Ogre::TEX_TYPE_2D);
			lTextureUnit->setTextureCoordSet(0);
			// The corresponding script :

/*
material M_NoLighting+PercentageOnlyOnOneTexture
{
	technique
	{
		pass
		{
			lighting off
			texture_unit
			{
				texture SimpleTexture2.bmp 2d
				tex_coord_set 1
				colour_op_ex blend_manual src_texture src_current 0.2
			}
			texture_unit
			{
				texture SimpleTexture.bmp 2d
				tex_coord_set 0
			}
		}
	}
}
*/
		}

		// Now I will create a ManualObject quad (as seen in previous tutorial).
		// There are some differences, because this time I give information about
		// texture coord and normal in the vertices.
		// I will create 2 set of texture coordinates, the first will be tiled (like building)
		// and the second won't (like a lightmap).
		Ogre::ManualObject * lManualObject = NULL;
		{
			Ogre::String lManualObjectName = "SomeQuad";
			lManualObject = lScene->createManualObject(lManualObjectName);

			// Always tell if you want to update the 3D (vertex/index) later or not.
			bool lDoIWantToUpdateItLater = false;
			lManualObject->setDynamic(lDoIWantToUpdateItLater);

			// BaseWhiteNoLighting is the name of a material that already exist inside Ogre.
			// Ogre::RenderOperation::OT_TRIANGLE_LIST is a kind of primitive.
			float lSize = 0.7f;
			lManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			{
				float cp = 1.0f * lSize ;
				float cm = -1.0f * lSize;
				float lDiffuseScale = 3.0f;
				float lLightmapScale = 1.0f;

				lManualObject->position(cm, cp, 0.0f);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
				lManualObject->normal(0.0, 0.0, 1.0f);
				lManualObject->textureCoord(0.0f, 0.0f);
				lManualObject->textureCoord(0.0f, 0.0f);

				lManualObject->position(cp, cp, 0.0f);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
				lManualObject->normal(0.0, 0.0, 1.0f);
				lManualObject->textureCoord(lDiffuseScale, 0.0f);
				lManualObject->textureCoord(lLightmapScale, 0.0f);

				lManualObject->position(cp, cm, 0.0f);// a vertex
				lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
				lManualObject->normal(0.0, 0.0, 1.0f);
				lManualObject->textureCoord(lDiffuseScale, lDiffuseScale);
				lManualObject->textureCoord(lLightmapScale, lLightmapScale);

				lManualObject->position(cm, cm, 0.0f);// a vertex
				lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
				lManualObject->normal(0.0, 0.0, 1.0f);
				lManualObject->textureCoord(0.0, lDiffuseScale);
				lManualObject->textureCoord(0.0, lLightmapScale);

				lManualObject->triangle(2,1,0);
				lManualObject->triangle(0,3,2);
			}
			lManualObject->end();
		}
		
		Ogre::String lNameOfTheMesh = "MyQuad";
		lManualObject->convertToMesh(lNameOfTheMesh);

		// I create an entity for each material, from left to right.
		std::vector< Ogre::String > lMaterialNames;
		lMaterialNames.push_back("M_Transparency");
		lMaterialNames.push_back("M_Decals");
		lMaterialNames.push_back("M_EnvMap");
		lMaterialNames.push_back("M_NoLighting+PercentageOnlyOnOneTexture");

		unsigned int lNumberOfEntities = lMaterialNames.size();
		for(unsigned int iter = 0; iter < lNumberOfEntities; ++iter)
		{
			const Ogre::String& lMaterialName = lMaterialNames[iter];

			Ogre::Entity* lEntity = lScene->createEntity(lNameOfTheMesh);
			// Now I attach it to a scenenode, so that it becomes present in the scene.
			Ogre::SceneNode* lNode = lRootSceneNode->createChildSceneNode(lMaterialName);
			lNode->attachObject(lEntity);
			// I move the SceneNode so that it is visible to the camera.
			float lPositionOffset = float(1+ iter * 2) - (float(lNumberOfEntities));
			lNode->translate(lPositionOffset, 0.0, -10.0f);
			lEntity->setMaterialName(lMaterialName);
		}
	}

	// cleaning of windows events managed by Ogre::WindowEventUtilities::...
	// I call it after a 'pause in window updating', in order to maintain smoothness.
	// Explanation : if you clicked 2000 times when the windows was being created, there are 
	// at least 2000 messages created by the OS to listen to. This is made to clean them.
	lRoot->clearEventTimes();

	// I wait until the window is closed.
	// The "message pump" thing is something you will see in most GUI application.
	// It allow the binding of messages between the application and the OS.
	// These messages are most of the time : keystroke, mouse moved, ... or window closed.
	// If I don't do this, the message are never caught, and the window won't close.
	while(!lOgreInit.mWindow->isClosed())
	{
		// Here I update the scene between 2 frames.
		{
			// I make the light rotate.
			Ogre::Degree lAngle(2.5);
			lLightSceneNode->yaw(lAngle);

			// I get the scenenode that is called M_EnvMap 
			// (I called it that way, see its creation)
			// and I make it rotate. 
			// It will better show the 'environment map' effect.
			Ogre::SceneNode* lSceneNode = lScene->getSceneNode("M_EnvMap");
			lSceneNode->yaw(lAngle * 0.03f);
		}

		// Drawings
		// the window update its content.
		// each viewport that is 'autoupdated' will be redrawn now,
		// in order given by its z-order.
		lWindow->update(false);

		// The drawn surface is then shown on the screen
		// (google "double buffering" if you want more details).
		// I always use vertical synchro.
		bool lVerticalSynchro = true;
		//lWindow->swapBuffers(lVerticalSynchro);
		lWindow->swapBuffers();
		// This update some internal counters and listeners.
		// Each render surface (window/rtt/mrt) that is 'auto-updated' has got its 'update' function called.
		lRoot->renderOneFrame();

		Ogre::WindowEventUtilities::messagePump();
	}

	// Let's cleanup!
	{
		lWindow->removeAllViewports();
	}
	{
		lScene->destroyAllCameras();
		lScene->destroyAllManualObjects();
		lScene->destroyAllEntities();
		lScene->destroyAllLights();
		lRootSceneNode->removeAndDestroyAllChildren();
	}
	{
		Ogre::ResourceGroupManager& lRgMgr = Ogre::ResourceGroupManager::getSingleton();
		lRgMgr.destroyResourceGroup(lNameOfResourceGroup);
	}

	return;
}


int main()
{
	try
	{
		AnOgreApplication();
		std::cout<<"end of the program"<<std::endl;
	}catch(Ogre::Exception &e)
	{
		MWARNING("!!!!Ogre::Exception!!!!\n"<<e.what());
	}catch(std::exception &e)
	{
		MWARNING("!!!!std::exception!!!!\n"<<e.what());
	}
	return 0;
}

