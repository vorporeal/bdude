#include "Enums.h"
#include "Player.h"

#include <Ogre.h>

Player::Player(Map *curMap)
: m_sceneNode(NULL), m_map(curMap), m_alive(false), m_moving(false), m_animAmount(0), m_movementSpline(NULL)
{
	// TODO: Create the player's scene node in a better way than adding and removing it.
	// Create a new node for the player, but remove it from the map, as it shouldn't exist until we spawn it.
	m_sceneManager = Ogre::Root::getSingleton().getSceneManager("MapSceneManager");
	m_sceneNode = m_sceneManager->getSceneNode("MapNode")->createChildSceneNode("PlayerNode");
	m_sceneManager->getSceneNode("MapNode")->removeChild(m_sceneNode->getName());

	// Create a sphere to represent the player (for the time being).  It gets its own scene node so that it can
	// be translated up 50 units relative to the player scene node.  (Remember, all objects fit in a 100x100x100
	// "tile" which has its origin at the center of its base.  Hence, we translate the sphere up 50 units so it is
	// centered in its cube.)
	Ogre::SceneNode *sphereNode = m_sceneNode->createChildSceneNode("SphereNode", Ogre::Vector3(0.0f, 50.0f, 0.0f));
	sphereNode->scale(0.5f, 0.5f, 0.5f);
	Ogre::Entity *sphere = m_sceneManager->createEntity(Ogre::SceneManager::PrefabType::PT_SPHERE);
	sphere->setMaterialName("Color/DarkGrey");
	sphereNode->attachObject(sphere);
}

Player::~Player(void)
{
	if(m_alive)
		m_sceneManager->getSceneNode("MapNode")->removeAndDestroyChild(m_sceneNode->getName());
	else
		delete m_sceneNode;
	m_sceneNode = NULL;

	delete m_movementSpline;
	m_movementSpline = NULL;
}

/**
 * This function will be used to spawn a player on the map whenever it is necessary.  It should go through the act of
 * resetting the player's position, statistics (if necessary), and anything else that must be set when the player is
 * spawned.
 */
void Player::spawnPlayer(Ogre::Vector3 mapPos, Ogre::Vector3 worldPos)
{
	m_worldPosition = worldPos;
	m_mapPosition = mapPos;

	mapPos *= 100;
	m_sceneNode->setPosition(mapPos.x, mapPos.y, mapPos.z);
	m_sceneManager->getSceneNode("MapNode")->addChild(m_sceneNode);

	m_alive = true;
	m_moving = false;
	m_animAmount = 0;
}

/**
 * This function will be used to clean up a player when they die.  It will do things such as coordinate death animations
 * and remove the player from the map.  When states get implemented, chances are that it will basically just change the
 * state of the player to "Dying" or similar.
 */
void Player::die(void)
{
	m_sceneManager->getSceneNode("MapNode")->removeChild(m_sceneNode->getName());
	m_alive = false;

	delete m_movementSpline;
	m_movementSpline = NULL;
}

/**
 * This function moves the player in the specified direction.  Care should be taken to only call this function
 * if it is known that the move is "legal".  This function doesn't do any bounds/error checking.  
 */
void Player::move(Direction dir)
{
	// If the player is already moving or is dead, don't try to move.
	if(!m_alive || m_moving)
		return;

	Ogre::Vector3 translateDir = Ogre::Vector3::ZERO;

	switch(dir)
	{
	case Direction::North:
		translateDir = Ogre::Vector3(0, 0, -1);
		break;
	case Direction::South:
		translateDir = Ogre::Vector3(0, 0, 1);
		break;
	case Direction::East:
		translateDir = Ogre::Vector3(1, 0, 0);
		break;
	case Direction::West:
		translateDir = Ogre::Vector3(-1, 0, 0);
		break;
	}

	// Set up a spline for smooth movement.
	m_movementSpline = new Ogre::SimpleSpline();
	m_movementSpline->addPoint(m_mapPosition * 100);
	
	// Adjust the stored map position to match the new position of the player.
	m_mapPosition += translateDir;

	// Add a new point onto the spline as an endpoint for the player.
	m_movementSpline->addPoint(m_mapPosition * 100);

	// Set the fact that we're moving.
	m_moving = true;
}

Ogre::Vector3 Player::getMapPosition() const
{
	return m_mapPosition;
}

Ogre::Vector3 Player::getWorldPosition() const
{
	return m_worldPosition;
}

void Player::update()
{
	// If we're not moving, there's no point in doing any of the movement stuff below.
	if(!m_moving)
		return;

	// Start by updating the distance we've moved along the spline.
	m_animAmount++;

	// Set the new position of the node.
	Ogre::Vector3 vec = m_movementSpline->interpolate(Ogre::Real((float) m_animAmount / (float) m_maxAnimTime));
	m_sceneNode->setPosition(vec.x, vec.y, vec.z);

	// This might be slow and/or unnecessary.  Should be revisited later.
	m_worldPosition = m_sceneNode->_getDerivedPosition();

	// If we're done moving, set values accordingly.
	if(m_animAmount == m_maxAnimTime)
	{
		// We're no longer moving.
		m_moving = 0;
		// Therefore, get rid of the movement spline.
		delete m_movementSpline;
		m_movementSpline = NULL;
		// Also, reset the amount we've moved along the spline.
		m_animAmount = 0;
	}
}