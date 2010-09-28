#pragma once

#include "Enums.h"
#include "Map.h"

#include "Ogre.h"

class Player
{
public:
	Player(Map *curMap);
	virtual ~Player(void);

	Ogre::Vector3 getMapPosition(void) const;
	Ogre::Vector3 getWorldPosition(void) const;

	// TODO: Figure out how a player should be spawned.
	// What should the arguments be?
	// What values does it need to set?
	void spawnPlayer(Ogre::Vector3 mapPos, Ogre::Vector3 worldPos);
	void die(void);

	// Move the player.  THIS IS A BIT OF A HACK, FIX IT LATER.
	// Actually, this isn't so bad.  Maybe doing things this way works.
	void move(Direction dir);

	const Ogre::SceneNode& getSceneNode() const;

protected:
	void setMapPosition(Ogre::Vector3 mapPos, bool updateWorld);
	void setWorldPosition(Ogre::Vector3 worldPos, bool updateMap);

private:
	Ogre::SceneNode *m_sceneNode;
	Ogre::SceneManager *m_sceneManager;

	Map *m_map;

	Ogre::Vector3 m_mapPosition;
	Ogre::Vector3 m_worldPosition;

	bool m_alive;
};