#pragma once
#pragma warning(disable : 5033)
#include <glm/glm.hpp>
#include <Tile.h>

// Map class
namespace Core
{
	class Map
	{
	public:
		// Takes a scale, the number of tiles in the x and y direction
		Map(std::string name, glm::vec2 scale, int tileSize, int width, int height);
		~Map();

		// Creates a
		void setMapTileSpritesFromArray(int *spriteIDMap);

		// Set mMapDimensions
		void setDimensions(glm::vec2 dimensions);

		// Get mMapDimensions
		glm::ivec2 getDimensions();

		// Set mTileSize
		void setTileSize(int size);

		// Get mMapDimensions
		int getTileSize();

		// Get mNumTiles
		int getNumTiles();

		void setName(std::string);
		std::string getName();

		friend class MapPage;

	private:
		int mwidth, mheight;
		// Set tile coordinates
		// (so we don't have to recalculate with each update)
		void setTileCoords();

		// For now, the map is an array of tiles with no depth
		Tile *mTileArray;

		// Size of the tile in pixels
		// (only one dimension because it's a square, x = y)
		int mTileSize;

		// Number of tiles, mMapDims.x * mMapDims.y.
		// This is stored just so we don't have to perform the multiplication
		// operation with each render
		int mNumTiles;

		// The dimensions of the map in tiles
		// (32, 32) means the map is 32 tiles by 32 tiles
		glm::ivec2 mMapDimensions;

		std::string mMapName;

		// This is set to a MapPage only when the map is in use on the MapPage.
		// It's used to remove itself from the MapPage if it's suddenly deleted
		// while in use.
		MapPage *mAssociatedPage;
	};
}