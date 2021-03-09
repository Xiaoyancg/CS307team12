#include "Map.h"

// Takes a scale, the number of tiles in the x and y direction 
Map::Map(glm::vec2 dimensions, int tileSize) :
mMapDimensions(dimensions),
mTileSize(tileSize) {
	// Create map 
	mMap = new Tile[mMapDimensions.x * mMapDimensions.y];
}

// Set mMapDimensions to new dimensions
void Map::setDimensions(glm::vec2 dimensions) {
	// setDimensions should copy the map to the new map! right now it just discards the old one
	delete mMap;
	Tile* newMap = new Tile[mMapDimensions.x * mMapDimensions.y];
	mMap = newMap;
	mMapDimensions = dimensions;
}

// Get mMapDimensions
glm::vec2 Map::getDimensions() {
	return mMapDimensions;
}