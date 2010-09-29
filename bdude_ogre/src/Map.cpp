#pragma warning( disable : 4482 )

#include "Enums.h"
#include "Player.h"
#include "MapIO.h"
#include "TerrainBlock.h"
#include "Map.h"

#include <Ogre.h>

#include <list>

/**
 * The objects array is stored as m_objects[y][x][z].  This is because lookups will frequently be within the same
 * height of the map, just different x/z coordinates, so having the y-coordinate first will likely make more efficient
 * use of cache space.  Because it's a pain to have to remember that the order is non-standard, this macro automatically
 * fixes the order and indexes into the object array.  This macro should be used whenever accessing the array to ensure
 * that elements are accessed in the correct order.
 */
#define OBJECTS(x, y, z) m_objects[(y)][(x)][(z)]

Map::Map(MapIO::MapData *data):
	m_mapTerrain(data->mapTerrain), m_sizeX((int)data->sizeX), m_sizeY((int)data->sizeY), m_sizeZ((int)data->sizeZ),
	m_worldX(data->worldX), m_worldY(data->worldY), m_worldZ(data->worldZ),
	// TODO: There could be issues here with initializing the multi_array with negative/0 sizes.
	m_objects(boost::extents[(int)data->sizeY][(int)data->sizeX][(int)data->sizeZ])
{
	// Make sure the grid size values are sane.  If not, fix them.
	m_sizeX = (m_sizeX > 0 ? m_sizeX : 1);
	m_sizeY = (m_sizeY > 0 ? m_sizeY : 1);
	m_sizeZ = (m_sizeZ > 0 ? m_sizeZ : 1);

	// Make sure the terrain type is a sane value.  If not, fix it.
	m_mapTerrain = (m_mapTerrain >= 0 && m_mapTerrain < MapTerrain::END ? m_mapTerrain : (MapTerrain) 0);

	// Initialize all of the block pointers to NULL so we know that there's nothing there.
	for(int y = 0; y < (int)data->sizeY; y++)
		for(int x = 0; x < (int)data->sizeX; x++)
			for(int z = 0; z < (int)data->sizeZ; z++)
				OBJECTS(x, y, z) = NULL;

	// HACK: This should be revisited for whether or not there's a better way to do this.  Having a separate
	// scene manager for the map may work well, as cleaning up after the map might be quite easy this way, although
	// I don't know enough about Ogre to really know if this is a good idea or not.  Either way, I threw it in to
	// get things working and it should be looked at again later.

	// Create the scene node for the map.  This will be the parent node for all map/terrain objects.
	m_sceneManager = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::SceneType::ST_GENERIC, "MapSceneManager");

	// TODO: Revisit whether it's necessary to save/load the world position of the map, instead of using the world origin by default.

	// Create a scene node to contain all of the level-specific terrain.  Certain things may exist outside of this node,
	// such as sky boxes, a sun, random "background" terrain, etc.
	m_sceneNode = m_sceneManager->getRootSceneNode()->createChildSceneNode("MapNode", Ogre::Vector3(m_worldX, m_worldY, m_worldZ));

	// Create blocks and add them to the map.
	std::list<MapIO::BlockData>::iterator iter;
	for(iter = data->blocks.begin(); iter != data->blocks.end(); iter++)
		OBJECTS(iter->gridX, iter->gridY, iter->gridZ) = new TerrainBlock(*iter);

	//////*** ALL CODE BELOW THIS POINT WILL REQUIRE REFACTORING ***//////
	// Most of it is very hackish, and is put here because I don't feel like dealing with stripping down
	// the BaseApplication code to only what I need.

	// Turn bounding boxes (for scene nodes) on or off.  Used for debugging purposes.
	//m_sceneManager->showBoundingBoxes(true);

	// Set the amount of ambient lighting in the scene.
	m_sceneManager->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));

	// Let's make a directional light for testing.
	Ogre::Light *directionalLight = m_sceneManager->createLight("directionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDirection(Ogre::Vector3(-2, -1, -3));
	directionalLight->setDiffuseColour(Ogre::ColourValue(0.25f, 0.25f, 0.25f));
	directionalLight->setSpecularColour(Ogre::ColourValue(0.25f, 0.25f, 0.25f));

	// Let's try creating a player!
	m_player = new Player(this);
	Ogre::Vector3 playerMapPos(0, 1, 4);
	m_player->spawnPlayer(playerMapPos, playerMapPos * 100 + m_sceneNode->_getDerivedPosition());

	// Let's load fun things!  Yay Jak's models!
	Ogre::SceneNode *dogNode = m_sceneNode->createChildSceneNode();
	dogNode->scale(1.6f, 1.6f, 1.6f);
	dogNode->translate(-40.0f, 105.0f, 70.0f);
	Ogre::Entity *dog = m_sceneManager->createEntity("dog.mesh");
	dogNode->attachObject(dog);

	Ogre::SceneNode *treeNode = m_sceneNode->createChildSceneNode();
	treeNode->translate(300.0f, 250.0f, 0.0f);
	Ogre::Entity *tree = m_sceneManager->createEntity("tree.mesh");
	treeNode->attachObject(tree);
}

Map::~Map(void)
{

}

/**
 * This function takes in a player and a direction, and checks to see if it's "legal" for
 * the player to move in that direction.  It does this by checking to see if there is an
 * object in the objects array at the location that the player wishes to move to.  If so,
 * the player may not move there.  This assumes that the objects array correctly represents
 * the state of the map.  If that assumption is false, we fucked up somewhere.
 */
bool Map::validMove(const Player &player, Direction dir) const
{
	Ogre::Vector3 newPlayerPos = player.getMapPosition();

	// The directions use a system where the negative Z-axis is North.
	switch(dir)
	{
	case Direction::North:
		newPlayerPos += Ogre::Vector3(0, 0, -1);
		break;
	case Direction::South:
		newPlayerPos += Ogre::Vector3(0, 0, 1);
		break;
	case Direction::East:
		newPlayerPos += Ogre::Vector3(1, 0, 0);
		break;
	case Direction::West:
		newPlayerPos += Ogre::Vector3(-1, 0, 0);
		break;
	}

	// Before checking if an object is there, make sure the location is on the map.
	// Otherwise, we'll get an "array index out of bounds exception".
	if(newPlayerPos.x >= m_sizeX || newPlayerPos.x < 0)
		return false;
	if(newPlayerPos.y >= m_sizeY || newPlayerPos.y < 0)
		return false;
	if(newPlayerPos.z >= m_sizeZ || newPlayerPos.z < 0)
		return false;

	// If the spot in the array is NULL (nothing there), the player may move there.
	return (OBJECTS((int) newPlayerPos.x, (int) newPlayerPos.y, (int) newPlayerPos.z) == NULL);
}

/**
 * This function does any tick-sensitive updating for all objects owned/controlled by the map.
 */
void Map::update(const Ogre::FrameEvent& evt)
{
	// TODO: Deal with the position returned by the player.
	// Right now, we just want to have the player update its position.
	Ogre::Vector3 curPos = m_player->getMapPosition();
	Ogre::Vector3 newPos = m_player->update(evt);
}

/**
 * This function is a small hack to avoid having to deal with multiple players (yet).  It does
 * what a non-hack version of this function would (check if the move is valid, and if so, move
 * the player), but it automatically calls it on the one player it currently knows about it.
 * This just makes it so I don't have to worry about checking which player the input belongs to
 * and passing that in to this function.
 */
void Map::movePlayerHACK(Direction dir)
{
	if(validMove(*m_player, dir))
		m_player->move(dir);
}