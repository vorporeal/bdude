#pragma once

#include <Ogre.h>

class MapObject
{
public:
	MapObject(void);
	virtual ~MapObject(void);

	virtual Ogre::Vector3 getMapPosition() const = 0;
};
