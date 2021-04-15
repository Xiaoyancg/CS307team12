#include "SpriteManager.h"
#include "FullSprite.h"
#include <iostream>
namespace Core
{
	SpriteManager::SpriteManager() : mCurrSpriteID(0), mCurrSpriteSheetID(0)
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
				Sprite *newSprite = new FullSprite(name, filename, id);
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
				Sprite *newSprite = new FullSprite(name, filename, id);
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
	std::unordered_map<int, Sprite *>& SpriteManager::getSprites()
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
			auto jSpriteList = j.get<std::vector<nlohmann::json>>();
			for (nlohmann::json& sj : jSpriteList)
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








	// SPRITESHEET STUFF

	int SpriteManager::createSpriteSheet(std::string name, std::string filename) {
		unsigned int id = mCurrSpriteSheetID;

		// This will loop as long as it needs to until it finds the closest valid ID
		while (true)
		{
			// Check if sprite with id does not already exist
			if (mSpriteSheets[id] == nullptr)
			{
				// init id for sprite
				SpriteSheet* newSpriteSheet = new SpriteSheet(name, filename, id);
				this->mSpriteSheets[id] = newSpriteSheet;
				mCurrSpriteSheetID = id + 1;
				return id; // Return ID of the new sprite
			}
			// Otherwise, the specified sprite ID already exists, so check the next ID
			else
			{
				printf("ID %d already used :( Trying SpriteSheetID: %d\n", id, id + 1);
				id++;
			}
		}
	}
	int SpriteManager::createSpriteSheet(std::string name, std::string filename, int id) {
		// This will loop as long as it needs to until it finds the closest valid ID
		while (true)
		{
			// Check if sprite with id does not already exist
			if (mSpriteSheets[id] == nullptr)
			{
				// init id for sprite
				SpriteSheet* newSpriteSheet = new SpriteSheet(name, filename, id);
				this->mSpriteSheets[id] = newSpriteSheet;
				return id; // Return ID of the new sprite
			}
			// Otherwise, the specified sprite ID already exists, so check the next ID
			else
			{
				printf("ID %d already used :( Trying SpriteSheetID: %d\n", id, id + 1);
				id++;
			}
		}
	}
	void SpriteManager::deleteSpriteSheet(int spritesheetID) {
		delete this->mSpriteSheets[spritesheetID];
		mSpriteSheets.erase(spritesheetID);
	}
	std::unordered_map<int, Sprite*>& SpriteManager::getSpriteSheets() {
		return this->mSpriteSheets;
	}
	Sprite* SpriteManager::atSheetID(int spritesheetID) {
		return mSpriteSheets[spritesheetID];
	}




}