#include "SpriteManager.h"

namespace Core
{
	// Creates a new sprite at the correct location in mSprites 
	int SpriteManager::createSprite(std::string filename) {
		this->mSprites[mCurrSpriteID] = new Sprite(filename);
		this->mCurrSpriteID++; // Increment the sprite ID so no sprite gets a duplicate ID
	}

	// Returns to a pointer to the sprite with the requested ID
	Sprite* SpriteManager::atID(int spriteID) {
		return mSprites[spriteID];
	}


}


