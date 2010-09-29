#pragma once

#include "MapObject.h"
#include "MapIO.h"

#include <Ogre.h>

class TerrainBlock : public MapObject
{
public:
	TerrainBlock(Ogre::Vector3 position);
	TerrainBlock(MapIO::BlockData &data);
	virtual ~TerrainBlock(void);

private:
	void createSceneNode(void);
	void loadContent(void);

	static Ogre::SceneManager *m_sceneMgr;

	Ogre::SceneNode *m_sceneNode;
	Ogre::Entity *m_entity;

	int m_gridx, m_gridy, m_gridz;
	BlockType m_blockType;
	BlockTerrain m_terrainType;
};
