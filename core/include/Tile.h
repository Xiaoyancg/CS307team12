#pragma once

#include <glm/glm.hpp>

/*
 * Tile class
 * Just stores a depth and sprite ID.
 * Maps are made of (map.dimension.x * map.dimension.y) of these tiles
 * Tiles are rendered from the map properly in a MapPage
 */

namespace Core
{
	class Tile
	{
	public:
		Tile() {}
		Tile(int spriteID, bool solid) : mSpriteID(spriteID), mSolid(solid) {}

		void setSpriteID(int id)
		{
			mSpriteID = id;
		}

		int getSpriteID()
		{
			return mSpriteID;
		}

		void setCoords(int *coords); // coords points to an array of 8 ints, 4 pairs of coordinates
		float *getCoords();

		void calculateSquareCoords(glm::vec2 position, glm::vec2 size);

		bool isSolid()
		{
			return mSolid;
		}
		void setSolid(bool solid)
		{
			mSolid = solid;
		}

	private:
		float mCoords[16]; // 4 pairs of (x,y) coordinates in pixels

		// Basic Tile variables
		int mCurrentDepth = 0; // Not implemented yet, but saving for later

		bool mSolid = false;

		// Sprite ID used to index sprites loaded into memory from the Game class
		// This isn't used yet though, I'm just adding it for later
		int mSpriteID = -2;
	};
}