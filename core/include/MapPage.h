#include "Page.h"
#include "Map.h"

// MapPage class
class MapPage : Page {
public:
	// Takes a scale, the number of tiles in the x and y direction 
	MapPage(SDL_Window* window);

	void setMap(Map* map);


	/////////////////////////////////////////////
	// Set the dimensions of the map (in tiles)
	void setMapDimensions(glm::vec2 dimensions);

	// Get the dimensions of the map (in tiles)
	glm::vec2 getMapDimensions();

	// Set the tile size of the map (in pixels)
	void setMapTileSize(int tileSize);

	// Get the tile size of the map (in pixels)
	int getMapTileSize();
	/////////////////////////////////////////////


	void render();

private:
	// The map! only one for now, I plan to add code to support 1 MapPage for an arbitrary amount of maps with std::vector and unique_ptrs
	Map* mMap;
};