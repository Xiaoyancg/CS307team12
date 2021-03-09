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
	// Show the current context
	SDL_GL_MakeCurrent(Page::mWindow, Page::mPageContext);

	for (int i = 0; i < mMap->getNumTiles(); i++) {
		printf("%d ", mMap->mTileArray[i].getSpriteID());
		if ((((i + 1) % (int) mMap->getDimensions().x) == 0) && i != 0) {
			printf("\n");
		}
	}
}