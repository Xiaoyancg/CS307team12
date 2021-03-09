#include <glm/glm.hpp>
#include <Tile.h>

// Map class
class Map {
public:
	// Takes a scale, the number of tiles in the x and y direction 
	Map(glm::vec2 scale, int tileSize);

	// Creates a 
	void setMapTileSpritesFromArray(int* spriteIDMap);

	// Set mMapDimensions
	void setDimensions(glm::vec2 dimensions);

	// Get mMapDimensions
	glm::vec2 getDimensions();

	// Set mTileSize
	void setTileSize(int size);

	// Get mMapDimensions
	int getTileSize();

	// Get mNumTiles
	int getNumTiles();

private:
	// For now, the map is an array of tiles with no depth
	Tile* mTileArray;

	// Size of the tile in pixels (only one dimension because it's a square, x = y)
	int mTileSize;

	// Number of tiles, mMapDims.x * mMapDims.y. This is stored just so we don't have to perform the multiplication operation with each render
	int mNumTiles;

	// The dimensions of the map in tiles
	glm::vec2 mMapDimensions; // (32, 32) means the map is 32 tiles by 32 tiles


	friend class MapPage;
};