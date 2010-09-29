#include "DynamicObject.h"

DynamicObject::DynamicObject(void)
: m_toRemove(false), m_toDestroy(false)
{
}

DynamicObject::~DynamicObject(void)
{
}

void DynamicObject::setRemove(void)
{
	m_toRemove = true;
}

void DynamicObject::setDestroy(void)
{
	m_toRemove = true;
	m_toDestroy = true;
}

bool DynamicObject::toRemove(void) const
{
	return m_toRemove;
}

bool DynamicObject::toDestroy(void) const
{
	return m_toDestroy;
}

void DynamicObject::objectAdded(void)
{
	m_toRemove = false;
}
