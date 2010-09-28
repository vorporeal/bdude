/*
-----------------------------------------------------------------------------
Filename:    bdude.cpp
-----------------------------------------------------------------------------
*/
#include "Map.h"
#include "MaterialSwitcher.h"
#include "bdude.h"

//-------------------------------------------------------------------------------------
bdude::bdude(void)
{
}
//-------------------------------------------------------------------------------------
bdude::~bdude(void)
{
}

//-------------------------------------------------------------------------------------
void bdude::createScene(void)
{
	/*mSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Entity *entNinja = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	entNinja->setCastShadows(true);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entNinja);

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 1500, 1500, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z); // TODO: LOOK THIS UP
	Ogre::Entity *entGround = mSceneMgr->createEntity("Ground", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround);
	entGround->setMaterialName("Ocean2_HLSL_GLSL");
	entGround->setCastShadows(false);

	// Skybox!
	mSceneMgr->setSkyBox(true, "SkyBox");

	// Lighting!
	/*Ogre::Light *pointLight = mSceneMgr->createLight("pointLight");
	pointLight->setType(Ogre::Light::LT_POINT);
	pointLight->setPosition(Ogre::Vector3(0, 150, 250));
	pointLight->setDiffuseColour(1.0f, 0.0f, 0.0f);
	pointLight->setSpecularColour(1.0f, 0.0f, 0.0f);*/

	/*Ogre::Light *directionalLight = mSceneMgr->createLight("directionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));
	directionalLight->setDiffuseColour(Ogre::ColourValue(0.25f, 0.25f, 0.0f));
	directionalLight->setSpecularColour(Ogre::ColourValue(0.25f, 0.25f, 0.0f));/

	/*Ogre::Light *spotLight = mSceneMgr->createLight("spotLight");
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setPosition(Ogre::Vector3(300, 300, 0));
	spotLight->setDirection(-1, -1, 0);
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));
	spotLight->setDiffuseColour(0.0f, 0.0f, 1.0f);
	spotLight->setSpecularColour(0.0f, 0.0f, 1.0f);*/

	// Hopefully this will create and display a map loaded from a map file.
	m_map = new Map(MapIO::parseMap("C:/Users/Stern/Desktop/map.txt"));
	mSceneMgr = mRoot->getSceneManager("MapSceneManager");
	createCamera();
	mWindow->removeAllViewports();
	createViewports();

	// This should allow for doing depth and normal rendering passes without changing material scripts.
	Ogre::MaterialManager::getSingleton().addListener(new MaterialSwitcher());

	// Let's try to get things to draw using the depth pass.
	Ogre::CompositorManager::getSingleton().addCompositor(m_viewport, "CelShadingCombined");
	Ogre::CompositorManager::getSingleton().setCompositorEnabled(m_viewport, "CelShadingCombined", true);

	mCamera->setAutoTracking(true, Ogre::Root::getSingleton().getSceneManager("MapSceneManager")->getSceneNode("SphereNode"));
}

void bdude::createCamera(void)
{
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setPosition(Ogre::Vector3(150, 700, 700));
	//mCamera->lookAt(Ogre::Vector3::ZERO);

	mCamera->setNearClipDistance(5);

	//mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

void bdude::createViewports(void)
{
	m_viewport = mWindow->addViewport(mCamera);

	m_viewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

	mCamera->setAspectRatio(Ogre::Real(m_viewport->getActualWidth()) / Ogre::Real(m_viewport->getActualHeight()));
}

bool bdude::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	else if (arg.key == OIS::KC_W)
	{
		m_map->movePlayerHACK(Direction::North);
	}
	else if (arg.key == OIS::KC_A)
	{
		m_map->movePlayerHACK(Direction::West);
	}
	else if (arg.key == OIS::KC_S)
	{
		m_map->movePlayerHACK(Direction::South);
	}
	else if (arg.key == OIS::KC_D)
	{
		m_map->movePlayerHACK(Direction::East);
	}

	return true;
}

bool bdude::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	// Update the map.
	m_map->update();

	mTrayMgr->frameRenderingQueued(evt);

	if (!mTrayMgr->isDialogVisible())
	{
		//mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
		}
	}

	return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        bdude app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
