#include "SpriteManager.h"

namespace Core
{
	int SpriteManager::addSprite(std::string filename) {
		mSprites[mCurrSpriteID] = new Sprite(filename);
		mCurrSpriteID++; // Increment the sprite ID so no sprite gets a duplicate ID
	}

	Sprite* SpriteManager::at(int spriteID) {
		return mSprites[spriteID];
	}


}


