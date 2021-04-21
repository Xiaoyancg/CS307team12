#include "Tile.h"
#include "glad/glad.h"
namespace Core
{
	// Copy the given coordinates to the coordinates of the tile
	void Tile::setCoords ( int *coords )
	{
		// Texture coordinates are set assuming a FullSprite, and will be handled differently in Map.cpp if not a FullSprite
		// P1
		mCoords[0] = coords[0];
		mCoords[1] = coords[1];
		// P1 texture coords
		mCoords[2] = 0;
		mCoords[3] = 1;

		// P2
		mCoords[4] = coords[2];
		mCoords[5] = coords[3];
		// P2 texture coords
		mCoords[6] = 0;
		mCoords[7] = 0;

		// P3
		mCoords[8] = coords[4];
		mCoords[9] = coords[5];
		// P3 texture coords
		mCoords[10] = 1;
		mCoords[11] = 1;

		// P4
		mCoords[12] = coords[6];
		mCoords[13] = coords[7];
		// P4 texture coords
		mCoords[14] = 1;
		mCoords[15] = 0;
	}

	float *Tile::getCoords ()
	{
		return mCoords;
	}

	// calculate tile coordinates with top-left corner on `position`
	void Tile::calculateSquareCoords(glm::vec2 position, glm::vec2 size) {
		int coords[8];
		coords[0] = position.x;
		coords[1] = position.y + size.y;

		coords[2] = position.x;
		coords[3] = position.y;

		coords[4] = position.x + size.x;
		coords[5] = position.y + size.y;

		coords[6] = position.x + size.x;
		coords[7] = position.y;
		setCoords(coords);
	}
}