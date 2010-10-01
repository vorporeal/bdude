#pragma once

enum Direction
{
	North,
	East,
	South,
	West
};

enum MapTerrain
{
	Grassy,
	Rocky,
	Desert,
	END
};

enum BlockType
{
	Flat,
	HalfSlope,
	QuarterSlope,
	HalfHeight,
	QuarterHeight,
	ThreeQuarterHeight,
	GEOMETRY_END
};

enum BlockTerrain
{
	Normal,
	Hazard,
	TERRAIN_END
};

// We can use bitwise operations to check whether we're destroying an object or just removing it.
enum ObjectState
{
	Alive = 0,
	ToRemove = 1,
	ToDestroy = 2
};