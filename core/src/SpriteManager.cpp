#include "SpriteManager.h"
#include <iostream>
namespace Core
{
	SpriteManager::SpriteManager() : mCurrSpriteID(0)
	{
	}

	// Creates a new sprite at the correct location in mSprites
	int SpriteManager::createSprite(std::string name, std::string filename)
	{
		unsigned int id = mCurrSpriteID;

		// This will loop as long as it needs to until it finds the closest valid ID
		while (true)
		{
			// Check if sprite with id does not already exist
			if (mSprites[id] == nullptr)
			{
				// init id for sprite
				Sprite *newSprite = new Sprite(name, filename, id);
				this->mSprites[id] = newSprite;
				mCurrSpriteID = id + 1;
				return id; // Return ID of the new sprite
			}
			// Otherwise, the specified sprite ID already exists, so check the next ID
			else
			{
				printf("ID %d already used :( Trying SpriteID: %d\n", id, id + 1);
				id++;
			}
		}
	}

	// Creates a new sprite at the correct location in mSprites
	int SpriteManager::createSprite(std::string name, std::string filename, int id)
	{
		// This will loop as long as it needs to until it finds the closest valid ID
		while (true)
		{
			// Check if sprite with id does not already exist
			if (mSprites[id] == nullptr)
			{
				// init id for sprite
				Sprite *newSprite = new Sprite(name, filename, id);
				this->mSprites[id] = newSprite;
				return id; // Return ID of the new sprite
			}
			// Otherwise, the specified sprite ID already exists, so check the next ID
			else
			{
				printf("ID %d already used :( Trying SpriteID: %d\n", id, id + 1);
				id++;
			}
		}
	}

	void SpriteManager::deleteSprite(int spriteID)
	{
		delete this->mSprites[spriteID];
		mSprites.erase(spriteID);
		//printSprites();
	}

	// Private function used for debugging
	void SpriteManager::printSprites()
	{
		printf("SPRITES:\n");
		for (const auto &[key, value] : mSprites)
		{
			printf(" > ID: %d, %p\n", key, value);
		}
	}

	// This may never be used, but if you need the whole list of sprites here it is!
	// This might be used to iterate through all sprites, but be warned that iteration with maps is in no specific order
	// Leaving it commented until a use case for it comes up
	std::unordered_map<int, Sprite *> SpriteManager::getSprites()
	{
		return this->mSprites;
	}

	// Returns to a pointer to the sprite with the requested ID
	Sprite *SpriteManager::atID(int spriteID)
	{
		return mSprites[spriteID];
	}

	int SpriteManager::parse(nlohmann::json j)
	{
		try
		{
			for (nlohmann::json sj : j)
			{
				createSprite(
					sj.at("SpriteName").get<std::string>(),
					sj.at("FileName").get<std::string>(),
					sj.at("SpriteID").get<int>());
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			return 1;
		}

		return 0;
	}
}