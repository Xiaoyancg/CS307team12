#include "Map.h"

// Takes a scale, the number of tiles in the x and y direction 
Map::Map(glm::vec2 dimensions, int tileSize) :
mMapDimensions(dimensions),
mTileSize(tileSize) {
	// Create map 
	mNumTiles = mMapDimensions.x * mMapDimensions.y;
	mTileArray = new Tile[mNumTiles];
}

// Set mMapDimensions to new dimensions
void Map::setDimensions(glm::vec2 dimensions) {
	// setDimensions should copy the map to the new map! right now it just discards the old one
	delete mTileArray;
	mNumTiles = mMapDimensions.x * mMapDimensions.y;
	Tile* newMap = new Tile[mNumTiles];
	mTileArray = newMap;
	mMapDimensions = dimensions;
}

// Get mMapDimensions
glm::vec2 Map::getDimensions() {
	return mMapDimensions;
}

void Map::setTileSize(int size) {
	mTileSize = size;
}

int Map::getTileSize() {
	return mTileSize;
}

int Map::getNumTiles() {
	return mNumTiles;
}

// Takes a pointer to an array of integers, containing the spriteID for each tile in the Map
// ASSUMES THE DIMENSIONS OF ARRAY spriteIDMap ARE THE SAME AS Map::mMapDimensions
// This will have to take a depth parameter when Tile depth gets implemented
void Map::setMapTileSpritesFromArray(int* spriteIDMap) {
	mNumTiles = mMapDimensions.x * mMapDimensions.y; // Get the number of tiles in the map

	for (int i = 0; i < mNumTiles; i++) {
		mTileArray[i].setSpriteID(spriteIDMap[i]); // Copy the spriteID to the Map at tile 'i'
		printf("> Map::setMapTileSpritesFromArray - Setting map tile %d to spriteID %d\n", i, spriteIDMap[i]);
	}
}
