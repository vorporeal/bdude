#pragma warning( disable : 4996 4482 )

#include "Map.h"
#include "MapIO.h"

#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <list>

MapIO::MapData* MapIO::parseMap(std::string path)
{
	// Create the input file stream for the map and tell it
	// to throw exceptions on failure.
	std::ifstream map_file;
	map_file.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	// Create the pointer to the map data.
	MapData *map;

	try
	{
		// Open the file for reading.
		map_file.open(path.c_str());

		// Create a new MapData object to read the map into.
		map = new MapData();

		// Create a buffer to read lines into.
		char buf[100];

		// Read grid size (x, y, and z).
		if(!map_file.getline(buf, 100) || sscanf(buf, "%d %d %d\n", &map->sizeX, &map->sizeY, &map->sizeZ) != 3)
			throw ParseException("Error: couldn't parse grid size from map file!");
		// Make sure grid size is sane (not 0, not negative).
		if(map->sizeX <= 0 || map->sizeY <= 0 || map->sizeZ <= 0)
			throw ParseException("Error: grid size invalid!");

		// Read world position (x, y, and z).
		if(!map_file.getline(buf, 100) || sscanf(buf, "%f %f %f\n", &map->worldX, &map->worldY, &map->worldZ) != 3)
			throw ParseException("Error: couldn't parse map position from map file!");

		// Read terrain type.
		if(!map_file.getline(buf, 100) || sscanf(buf, "%d\n", &map->mapTerrain) != 1)
			throw ParseException("Error: couldn't parse map terrain type from map file!");
		// Make sure terrain type is sane.
		if(map->mapTerrain < 0 || map->mapTerrain >= MapTerrain::END)
			throw ParseException("Error: invalid map terrain type!");

		/// Define some variables that will be used in the loop for parsing blocks. ///

		// gridX is -1 because it will be incremented to 0 if the first block has no defined position.
		int gridX = -1, gridY = 0, gridZ = 0;
		BlockType geomType;
		BlockTerrain terrainType;
		
		// Variables for testing if there is a colon present.
		int len, colon;

		// This pointer will point to the part of the string after the colon, if one is present.
		char *post_colon;

		// Get the next line from the map file.  This should be the first block.
		map_file.getline(buf, 100);

		// Loop through the block data.
		while(!map_file.eof() && strcmp(buf, "MAPEND"))
		{
			// Make sure the string isn't empty.  If so, skip it.
			if(strlen(buf) == 0)
			{
				map_file.getline(buf, 100);
				continue;
			}

			// Set geometry and terrain type variables to impossible values.
			geomType = (BlockType) -1;
			terrainType = (BlockTerrain) -1;

			// Set post_colon to the beginning of the buffer (so we can use it if there is no colon).
			post_colon = buf;

			// Get length of the string.
			len = strlen(buf);

			// Check for presence of colon in token.
			colon = strcspn(buf, ":");
			// If colon present, parse the grid data and get a pointer to the char after it (for later).
			if(colon != len)
			{
				if(sscanf(buf, "(%d, %d, %d) :", &gridX, &gridY, &gridZ) != 3)
					throw ParseException("ERROR: Unable to parse grid location for block!");
				post_colon = strchr(buf, ':') + sizeof(char);
			}
			// If not present, calculate next grid location using last location.
			else
				incrementGridLocation(map, gridX, gridY, gridZ);

			// Make sure the grid location is valid.
			if(!testGridLocation(map, gridX, gridY, gridZ))
				throw ParseException("ERROR: Invalid grid location for block!");

			// Parse block geometry and terrain types.
			if(sscanf(post_colon, "%d %d", &geomType, &terrainType) != 2)
				throw ParseException("ERROR: Couldn't parse geometry or terrain type for block!");
			if(geomType < 0 || geomType >= BlockType::GEOMETRY_END)
				throw ParseException("ERROR: Invalid geometry type for block!");
			if(terrainType < 0 || terrainType >= BlockTerrain::TERRAIN_END)
				throw ParseException("ERROR: Invalid terrain type for block!");

			// Create new BlockData object with parsed values and add it to the list.
			BlockData block(gridX, gridY, gridZ, geomType, terrainType);
			// std::cout << block.to_str() << std::endl;
			map->blocks.push_back(block);

			// Get the next line from the map file.
			map_file.getline(buf, 100);
		}
	}
	catch(std::exception &e)
	{
		map_file.close();
		if(!map_file.eof())
		{
			std::cout << e.what() << std::endl;
			return NULL;
		}
		else
		{
			// TODO: Remove testing code.
			//// TESTING TESTING TESTING ////
			std::cout << map->to_str() << std::endl;
			return map;
		}
	}

	// TODO: Remove testing code.
	//// TESTING TESTING TESTING ////
	std::cout << map->to_str() << std::endl;

	map_file.close();
	return map;
}

void MapIO::saveMap(std::string path, MapIO::MapData *map)
{
	// Open file for writing the map to.
	std::ofstream map_file;
	map_file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	try
	{
		map_file.open(path.c_str());
	
		// Write the map size to the file.
		map_file << map->sizeX << " " << map->sizeY << " " << map->sizeZ << std::endl;
	
		// Write the map position to the file.
		map_file << map->worldX << " " << map->worldY << " " << map->worldZ << std::endl;
	
		// Write the map terrain to the file.
		map_file << map->mapTerrain << std::endl << std::endl;
	
		// Iterate through the blocks in the map and write them to the file.
		std::list<MapIO::BlockData>::iterator it;
		for(it = map->blocks.begin(); it != map->blocks.end(); it++)
		{
			// TODO: Optimize saving so it doesn't write the block position every time.
			// Write the block position data to the file.
			map_file << "(" << it->gridX << ", " << it->gridY << ", " << it->gridZ << "): ";
	
			// Write the geometry and terrain data to the file.
			map_file << it->blockType << " " << it->blockTerrain << std::endl;
		}
	
		// Write the MAPEND to the file.
		map_file << "MAPEND" << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		map_file.close();
	}
}

void MapIO::incrementGridLocation(MapIO::MapData *map, int &gridX, int &gridY, int &gridZ)
{
	gridX++;
	if(gridX >= map->sizeX)
	{
		gridX = 0;
		gridZ++;
		if(gridZ >= map->sizeZ)
		{
			gridZ = 0;
			gridY++;
		}
	}
}

bool MapIO::testGridLocation(MapIO::MapData *map, int gridX, int gridY, int gridZ)
{
	if(gridX >= map->sizeX ||
	   gridZ >= map->sizeZ ||
	   gridY >= map->sizeY)
	   return false;
	return true;
}