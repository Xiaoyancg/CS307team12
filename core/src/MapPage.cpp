#include "MapPage.h"
#include "glm/glm.hpp"

 
MapPage::MapPage(SDL_Window* window) :
Page(window) {
	mMap = new Map(glm::vec2(0, 0), 0);
}

MapPage::MapPage(SDL_Window* window, Map* map) :
	Page(window) {
	mMap = map;
}



// Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
void MapPage::setMap(Map* map) {
	mMap = map;
}

void MapPage::render() {

}