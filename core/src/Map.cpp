#include "Map.h"

// Takes a scale, the number of tiles in the x and y direction 
Map::Map(glm::vec2 dimensions, int tileSize) :
mMapDimensions(dimensions),
mTileSize(tileSize) {
	// Create map 
	mTileArray = new Tile[mMapDimensions.x * mMapDimensions.y];
}

// Set mMapDimensions to new dimensions
void Map::setDimensions(glm::vec2 dimensions) {
	// setDimensions should copy the map to the new map! right now it just discards the old one
	delete mTileArray;
	Tile* newMap = new Tile[mMapDimensions.x * mMapDimensions.y];
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




void Map::setMapTileSpritesFromArray(int* spriteIDMap, int spriteMapWidth, int spriteMapHeight) {
	int numTiles = spriteMapWidth * spriteMapHeight;
	mTileArray = new Tile[numTiles];

	for (int i = 0; i < numTiles; i++) {
		mTileArray[i].setSpriteID(spriteIDMap[i]);
		printf("> Map::setMapTileSpritesFromArray - Setting map tile %d to spriteID %d\n", i, spriteIDMap[i]);
	}
}
