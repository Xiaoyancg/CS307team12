#pragma once
#include <string>
#include <unordered_map>
#include "Sprite.h"


namespace Core
{
	// Sprite class
	class SpriteManager
	{
	public:
		// Creates a new sprite based on the filename and returns the new sprite's ID
		int addSprite(std::string filename); 

		// Returns a pointer to the sprite with the requested ID
		// (in constant time because we're using unordered_maps instead of vectors!)
		Sprite* at(int spriteID);

	private:
		int mCurrSpriteID;

		std::unordered_map<int, Sprite*> mSprites;
	};
}