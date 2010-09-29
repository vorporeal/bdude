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

	// Returns true if this DynamicObject should be removed.
	bool toRemove(void) const;

	// Returns true if this DynamicObject should be destroyed.
	bool toDestroy(void) const;

	// Should be called when the object is added to the list of dynamic objects.
	void objectAdded(void);

protected:
	void setRemove(void);
	void setDestroy(void);

private:
	bool m_toRemove;
	bool m_toDestroy;
};
