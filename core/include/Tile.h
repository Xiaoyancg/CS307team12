#pragma once
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
		// Sets the spriteID to -1
		Tile ();

		void setSpriteID ( int id )
		{
			mSpriteID = id;
		}

		int getSpriteID ()
		{
			return mSpriteID;
		}

		void setCoords ( int *coords ); // coords points to an array of 8 ints, 4 pairs of coordinates
		int *getCoords ();


	private:
		int mCoords[16]; // 4 pairs of (x,y) coordinates in pixels

		// Basic Tile variables
		int mCurrentDepth; // Not implemented yet, but saving for later

		// Sprite ID used to index sprites loaded into memory from the Game class
		// This isn't used yet though, I'm just adding it for later
		int mSpriteID = -1;
	};
}