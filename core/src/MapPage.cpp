#include "MapPage.h"
#include "glm/glm.hpp"
#include "Game.h" // Needed to setup shaders. New OpenGL context needs new shaders
#include <string>

 
MapPage::MapPage(SDL_Window* window) :
Page(window) {
	mMap = new Map(glm::vec2(0, 0), 0);
	initShaders(); // New OpenGL context created, so we need to set up the shaders for this context
}

MapPage::MapPage(SDL_Window* window, Map* map) :
Page(window) {
	mMap = map;
	initShaders(); // New OpenGL context created, so we need to set up the shaders for this context
}



// Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
void MapPage::setMap(Map* map) {
	mMap = map;
}

void MapPage::render() {
	// Show the current context
	SDL_GL_MakeCurrent(mWindow, mPageContext);
	glClearColor(0.1, 0.9, 0.59, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Traverse all tiles in the Map
	for (int i = 0; i < mMap->getNumTiles(); i++) {
		// Render each tile of the map!
		int* coords = mMap->mTileArray[i].getCoords(); // Get ptr to the tile coordinates

		// Buffer and draw tile
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(int), coords, GL_DYNAMIC_DRAW); 
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	SDL_GL_SwapWindow(mWindow); // Show the buffer by bringing it to the front
}