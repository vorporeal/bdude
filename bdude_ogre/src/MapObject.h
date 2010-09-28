#pragma once

class MapObject
{
public:
	MapObject(void);
	virtual ~MapObject(void);

	virtual inline bool isDestructible(void) = 0;
};
