#pragma once

#include "Enums.h"

#include <Ogre.h>

class MapObject
{
public:
	MapObject(void);
	virtual ~MapObject(void);

	virtual Ogre::Vector3 getMapPosition() const = 0;

	ObjectState getState(void) const;
	void setState(ObjectState state);

private:
	ObjectState m_state;
};
