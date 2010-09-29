#pragma warning( disable : 4482 )

#include <Ogre.h>

#include "Bomb.h"
#include "DynamicObject.h"
#include "IDestructible.h"
#include "Map.h"

const float Bomb::m_animTime = 3.0f;

Bomb::Bomb(Map *curMap, Ogre::Vector3 mapPos) :
m_map(curMap), m_mapPosition(mapPos), m_animAmount(0.0f)
{
	m_sceneMgr = Ogre::Root::getSingleton().getSceneManager("MapSceneManager");

	m_sceneNode = m_sceneMgr->getSceneNode("MapNode")->createChildSceneNode(m_mapPosition * 100);
	m_sceneNode->scale(0.3f, 0.3f, 0.3f);
	
	//Ogre::SceneNode *entNode = m_sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 7.5f, 0.0f));
	Ogre::SceneNode *entNode = m_sceneNode->createChildSceneNode(Ogre::Vector3(0.0f, 7.5f, 0.0f));
	Ogre::Entity *ent = m_sceneMgr->createEntity(Ogre::SceneManager::PrefabType::PT_SPHERE);
	entNode->attachObject(ent);
	ent->setMaterialName("Color/Maroon");
}

Bomb::~Bomb(void)
{
	m_sceneMgr->destroySceneNode(m_sceneNode);
}

Ogre::Vector3 Bomb::update(const Ogre::FrameEvent& evt)
{
	m_animAmount += evt.timeSinceLastFrame;

	if(m_animAmount >= m_animTime)
		this->destroy();

	return m_mapPosition;
}

void Bomb::destroy()
{
	m_sceneMgr->getSceneNode("MapNode")->removeChild(m_sceneNode);

	this->DynamicObject::setDestroy();
}

Ogre::Vector3 Bomb::getMapPosition() const
{
	return m_mapPosition;
}