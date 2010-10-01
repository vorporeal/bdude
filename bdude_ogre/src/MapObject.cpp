#pragma warning( disable : 4482 )

#include "Enums.h"
#include "MapObject.h"

MapObject::MapObject(void)
: m_state(ObjectState::Alive)
{
}

MapObject::~MapObject(void)
{
}

ObjectState MapObject::getState(void) const
{
	return m_state;
}

void MapObject::setState(ObjectState state)
{
	m_state = state;
}