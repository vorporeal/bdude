#include "MapIO.h"

#include <iostream>

int main(int argc, char *argv[])
{
	MapIO::MapData *map = MapIO::parseMap("C:/Users/Stern/Desktop/map.txt");
	MapIO::saveMap("C:/Users/Stern/Desktop/map_save.txt", map);
	return (int) MapIO::parseMap("C:/Users/Stern/Desktop/map_save.txt");
}