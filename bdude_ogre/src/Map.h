#pragma once

#include "Enums.h"
#include "MapIO.h"

#include <Ogre.h>

#include <boost/multi_array.hpp>

#include <string>

class Player;
class MapObject;

class Map
{
public:
	Map(MapIO::MapData *data);
	virtual ~Map(void);

	bool validMove(const Player &player , Direction dir) const;
	void movePlayerHACK(Direction dir);

private:
	typedef boost::multi_array<MapObject*, 3> ObjectArray;
	ObjectArray m_objects;

	Ogre::SceneManager *m_sceneManager;
	Ogre::SceneNode *m_sceneNode;

	Player *m_player;

	MapTerrain m_mapTerrain;
	int m_sizeX, m_sizeY, m_sizeZ;
	float m_worldX, m_worldY, m_worldZ;
};