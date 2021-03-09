#include "MapPage.h"
#include "glm/glm.hpp"


// Takes a scale, the number of tiles in the x and y direction 
MapPage::MapPage(SDL_Window* window) :
Page(window) {
	mMap = new Map(glm::vec2(0, 0), 0);
}

// Sets mMap to the given map (map should be created with 'new Map' on the heap)
void MapPage::setMap(Map* map) {
	mMap = map;
}

/////////////////////////////////////////////
// Set the dimensions of the map (in tiles)
void MapPage::setMapDimensions(glm::vec2 dimensions) {
	mMap.setDimensions(dimensions);
}

// Get the dimensions of the map (in tiles)
glm::vec2 MapPage::getMapDimensions() {
	return mMap.getDimensions();
}

// Set the tile size of the map (in pixels)
void MapPage::setMapTileSize(int tileSize) {
	mMap.setTileSize(tileSize);
}

// Get the tile size of the map (in pixels)
int MapPage::getMapTileSize() {
	return mMap.getTileSize();
}
/////////////////////////////////////////////


void MapPage::render() {

}