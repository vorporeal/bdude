#pragma warning( disable : 4482 )

#include <Ogre.h>

#include "Bomb.h"
#include "DynamicObject.h"
#include "IDestructible.h"
#include "Map.h"

#include <cmath>

const float Bomb::m_animTime = 2.0f;

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

	// Set spline points for animation.
	m_spline.addPoint(m_mapPosition * 100);
	m_spline.addPoint(m_mapPosition * 100 + Ogre::Vector3(0.0f, 50.0f - std::fmod(m_mapPosition.y, 50.0f), 0.0f));
}

Bomb::~Bomb(void)
{
	m_sceneMgr->destroySceneNode(m_sceneNode);
}

Ogre::Vector3 Bomb::update(const Ogre::FrameEvent& evt)
{
	m_animAmount += evt.timeSinceLastFrame;

	Ogre::Vector3 vec = m_spline.interpolate(m_animAmount / m_animTime);
	m_sceneNode->setPosition(vec.x, vec.y, vec.z);

	if(m_animAmount >= m_animTime)
		this->destroy();

	return m_mapPosition;
}

void Bomb::destroy()
{
	m_sceneMgr->getSceneNode("MapNode")->removeChild(m_sceneNode);

	this->MapObject::setState(ObjectState::ToDestroy);
	m_map->removeObject(this);
}

Ogre::Vector3 Bomb::getMapPosition() const
{
	return m_mapPosition;
}