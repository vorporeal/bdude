#pragma once
#pragma warning( disable : 4482 )

#include "Enums.h"

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <stdexcept>

class MapIO
{
public:
	struct BlockData
	{
		int gridX, gridY, gridZ;
		BlockTerrain blockTerrain;
		BlockType blockType;

		BlockData(int x, int y, int z, BlockType type, BlockTerrain terr)
			: gridX(x), gridY(y), gridZ(z), blockType(type), blockTerrain(terr) {}
	};

	struct MapData
	{
		int sizeX, sizeY, sizeZ;
		float worldX, worldY, worldZ;
		MapTerrain mapTerrain;
		std::list<BlockData> blocks;

		std::string to_str(void)
		{
			std::ostringstream ret;
			ret << "MAP DATA:" << std::endl;
			ret << "    Size: (" << sizeX << ", " << sizeY << ", " << sizeZ << ")" << std::endl;
			ret << "    Position: (" << worldX << ", " << worldY << ", " << worldZ << ")" << std::endl;
			ret << "    Terrain: ";
			switch(mapTerrain)
			{
			case MapTerrain::Grassy:
				ret << "Grassy" << std::endl;
				break;
			case MapTerrain::Rocky:
				ret << "Rocky" << std::endl;
				break;
			case MapTerrain::Desert:
				ret << "Desert" << std::endl;
				break;
			default:
				ret << "ERROR" << std::endl;
			}
			ret << "    Contains " << blocks.size() << " blocks." << std::endl;
			return ret.str();
		}
	};


	static MapData* parseMap(std::string path);
	static void saveMap(std::string path, MapData *map);

private:
	static void incrementGridLocation(MapData *map, int &gridX, int &gridY, int &gridZ);
	static bool testGridLocation(MapData *map, int gridX, int gridY, int gridZ);
};

// Let's define an exception that we can thrown when parsing fails.
class ParseException : public std::runtime_error
{
public:
	ParseException::ParseException(char *error) : std::runtime_error(error) {}
};