#pragma once
#include <string>
#include <unordered_map>
#include "Sprite.h"


/*
 * Use the SpriteManager when using sprites
 * 
 * This will handle constant-time access of sprites based on their ID ( using SpriteManager::atID(id) )
 * as well as set the each sprite ID automatically. 
 */

namespace Core
{
	// SpriteManager class
	class SpriteManager
	{
	public:
		// Creates a new sprite based on the filename and returns the new sprite's ID
		int createSprite(std::string filename); 

		// Returns a pointer to the sprite with the requested ID
		// (in constant time because we're using unordered_maps instead of vectors!)
		Sprite* atID(int spriteID);

	private:
		int mCurrSpriteID; // The current sprite ID, which will be set to whatever sprite is created next

		// This is where sprites are stored, like an array based on their IDs
		// If a sprite has ID=32, then a pointer to it is stored at mSprites[32]
		std::unordered_map<int, Sprite*> mSprites; 
	};
}