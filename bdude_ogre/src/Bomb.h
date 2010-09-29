#pragma once

#include <Ogre.h>

#include "DynamicObject.h"
#include "IDestructible.h"

class Map;

class Bomb : public DynamicObject, public IDestructible
{
public:
	Bomb(Map *map, Ogre::Vector3 pos);
	virtual ~Bomb(void);

	virtual Ogre::Vector3 update(const Ogre::FrameEvent& evt);
	virtual void destroy(void);

	virtual Ogre::Vector3 getMapPosition() const;

private:
	Ogre::SceneManager *m_sceneMgr;
	Ogre::SceneNode *m_sceneNode;

	Map *m_map;

	float m_animAmount;
	static const float m_animTime;

	Ogre::Vector3 m_mapPosition;
};
