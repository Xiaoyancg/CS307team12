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

	// calculate starting pos and loop through map
	// Get 'origin'/center of map (in tiles)
	glm::vec2 mapDims = mMap->getDimensions();
	glm::vec2 arrayOrigin(mapDims.x / 2, mapDims.y / 2);

	// Calculate the top left corner of the map using the arrayOrigin (in pixels)
	glm::vec2 topLeftCornerPx(arrayOrigin.x * -mMap->mTileSize, arrayOrigin.y * mMap->mTileSize);

	for (int i = 0; i < mMap->getNumTiles(); i++) {
		printf("%d ", mMap->mTileArray[i].getSpriteID());
		if ((((i + 1) % (int) mMap->getDimensions().x) == 0) && i != 0) {
			printf("\n");
		}
	}

	/*
	// TODO: Draw tiles of the map!
	// Load the data of the 'coords' buffer into the currently bound array buffer, VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(mCoords), mCoords, GL_DYNAMIC_DRAW);

	// Draw the bound buffer (coords)
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	*/
}