#include "SpriteManager.h"

namespace Core
{
	// Creates a new sprite at the correct location in mSprites 
	int SpriteManager::createSprite(std::string filename) {
		int id = mCurrSpriteID;
		printf("with ID %d... ", id);
		this->mSprites[id] = new Sprite(filename);
		this->mCurrSpriteID++; // Increment the sprite ID so no sprite gets a duplicate ID
		return id;
	}

	void SpriteManager::deleteSprite(int spriteID) {
		delete this->mSprites[spriteID];
		mSprites.erase(spriteID);
	}

	// This may never be used, but if you need the whole list of sprites here it is!
	// This might be used to iterate through all sprites, but be warned that iteration with maps is in no specific order
	// Leaving it commented until a use case for it comes up
	/*
	std::unordered_map<int, Sprite*> SpriteManager::getSprites() {
		return this->mSprites;
	}
	*/


	// Returns to a pointer to the sprite with the requested ID
	Sprite* SpriteManager::atID(int spriteID) {
		return mSprites[spriteID];
	}


}


