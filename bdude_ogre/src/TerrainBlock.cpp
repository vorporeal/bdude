#pragma warning( disable : 4482 )

#include "Enums.h"
#include "TerrainBlock.h"
#include "MapIO.h"

#include <Ogre.h>

// As the scene manager is a static variable, it needs to be initialized in the .cpp file outside of
// any constructors or methods.
Ogre::SceneManager* TerrainBlock::m_sceneMgr = NULL;

TerrainBlock::TerrainBlock(Ogre::Vector3 position):
	m_mapPosition(position), m_blockType(BlockType::Flat), m_terrainType(BlockTerrain::Normal)
{
		createSceneNode();
		loadContent();
}

TerrainBlock::TerrainBlock(MapIO::BlockData &data):
m_mapPosition(Ogre::Vector3(Ogre::Real(data.gridX), Ogre::Real(data.gridY), Ogre::Real(data.gridZ))),
m_blockType(data.blockType), m_terrainType(data.blockTerrain)
{
	createSceneNode();
	loadContent();
}

TerrainBlock::~TerrainBlock(void)
{
	
}

/**
 * This function creates the scene node for the TerrainBlock based on the map position it was created with.
 * It uses the fact that node's are placed relative to the parent node to get the node for the terrain block
 * in the correct place, as we only need to know the map position (and size of a block) to correctly place
 * a TerrainBlock, as it will automatically compensate for any "starting position" for the map.
 */
void TerrainBlock::createSceneNode()
{
	// Set the scene manager variable, if it hasn't been set yet.
	if(!m_sceneMgr)
		m_sceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("MapSceneManager");

	// Create the scene node for the TerrainBlock, using MapNode as the parent.
	m_sceneNode = m_sceneMgr->getSceneNode("MapNode")->createChildSceneNode(Ogre::Vector3(100) * m_mapPosition);
}

void TerrainBlock::loadContent()
{
	// TODO: Load data for terrain blocks!!!  VERY IMPORTANT
	// Right now, just use a switch statement to load basic meshes.
	switch(m_blockType)
	{
	case BlockType::Flat:
		m_entity = m_sceneMgr->createEntity("cube.mesh");
		break;
	case BlockType::HalfHeight:
		m_entity = m_sceneMgr->createEntity("half_cube.mesh");
		break;
	case BlockType::HalfSlope:
		m_entity = m_sceneMgr->createEntity("slope.mesh");
		break;
	case BlockType::QuarterSlope:
		m_entity = m_sceneMgr->createEntity("half_slope.mesh");
		break;
	// TODO: Come up with a better default case for loading block models.
	// There (eventually) shouldn't be a need for one, but one should exist nevertheless.
	default:
		m_entity = m_sceneMgr->createEntity("cube.mesh");
		break;
	}
	
	// Set the material for the block to a standard, light grey (for the time being).
	m_entity->setMaterialName("Color/LightGrey");

	// Attach the block to the scene node.
	m_sceneNode->attachObject(m_entity);
}

Ogre::Vector3 TerrainBlock::getMapPosition() const
{
	return m_mapPosition;
}
