#pragma once

#include "MapObject.h"

#include <Ogre.h>

class DynamicObject :
	public MapObject
{
public:
	DynamicObject(void);
	virtual ~DynamicObject(void);
	
	// Update the object's state.
	virtual Ogre::Vector3 update(const Ogre::FrameEvent& evt) = 0;

	// Get the object's position in map coordinates.
	virtual Ogre::Vector3 getMapPosition() const = 0;
};
