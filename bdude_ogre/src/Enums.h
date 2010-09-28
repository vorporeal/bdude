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