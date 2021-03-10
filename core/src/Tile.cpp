#include "Tile.h"
#include "glad/glad.h"

// Sets the spriteID (eventually mCurrentDepth too)
Tile::Tile() :
	mSpriteID(-1) {
	// Nothing needs to be done here
	// Sprite ID gets set to -1 by default, just meaning the sprite hasn't been set
}

// Copy the given coordinates to the coordinates of the tile
void Tile::setCoords(int * coords) {
	mCoords[0] = coords[0];
	mCoords[1] = coords[1];
	mCoords[2] = coords[2];
	mCoords[3] = coords[3];
	mCoords[4] = coords[4];
	mCoords[5] = coords[5];
	mCoords[6] = coords[6];
	mCoords[7] = coords[7];
}

void Tile::render() {
	glBufferData(GL_ARRAY_BUFFER, sizeof(mCoords), mCoords, GL_DYNAMIC_DRAW); // Buffer the 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int* Tile::getCoords() {
	return mCoords;
}