#include <glm/glm.hpp>
#include <Tile.h>

// Map class
class Map {
public:
	// Takes a scale, the number of tiles in the x and y direction 
	Map(glm::vec2 scale, int tileSize);

	// Set mMapDimensions
	void setDimensions(glm::vec2 dimensions);

	// Get mMapDimensions
	glm::vec2 getDimensions();

private:
	// The map is an array of tiles
	Tile* mMap;

	// Size of the tile in pixels (only one dimension because it's a square, x = y)
	int mTileSize;

	// The dimensions of the map in tiles
	glm::vec2 mMapDimensions;
};