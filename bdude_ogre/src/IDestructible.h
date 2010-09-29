#pragma once

class IDestructible
{
public:
	virtual ~IDestructible() {};

	virtual void destroy() = 0;

protected:
	IDestructible() {};
};