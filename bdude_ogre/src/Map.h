#pragma once

#include "Enums.h"
#include "MapIO.h"

#include <Ogre.h>

#include <boost/multi_array.hpp>

#include <string>
#include <list>

class Player;
class MapObject;
class DynamicObject;

class Map
{
public:
	Map(MapIO::MapData *data);
	virtual ~Map(void);

	bool validMove(const Player &player , Direction dir) const;
	void movePlayerHACK(Direction dir);
	void dropBombHACK();

	void update(const Ogre::FrameEvent& evt);

	//TODO: Decide whether these should be public/protected/private.
	void addPlayer(Player *player);
	void removePlayer(Player *player);

	bool addObject(MapObject *obj);
	bool removeObject(MapObject *obj);

	MapObject* getObject(Ogre::Vector3 pos) const;

private:
	typedef boost::multi_array<MapObject*, 3> ObjectArray;
	ObjectArray m_objects;
	std::list<DynamicObject*> m_dynamicObjects;
	std::list<Player*> m_players;

	std::queue<DynamicObject*> m_removables;

	Ogre::SceneManager *m_sceneManager;
	Ogre::SceneNode *m_sceneNode;

	Player *m_player;

	MapTerrain m_mapTerrain;
	int m_sizeX, m_sizeY, m_sizeZ;
	float m_worldX, m_worldY, m_worldZ;
};