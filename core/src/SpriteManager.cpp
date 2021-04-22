#include "SpriteManager.h"
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
				//printf("ID %d already used :( Trying SpriteID: %d\n", id, id + 1);
				id++;
			}
		}
	}

	// Creates a new sprite at the given location in mSprites (may overwrite)
	int SpriteManager::createSprite(std::string name, std::string filename, int id)
	{
		// init id for sprite
		Sprite *newSprite = new FullSprite(name, filename, id);
		this->mSprites[id] = newSprite;
		return id; // Return ID of the new sprite
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
	std::unordered_map<int, Sprite *> &SpriteManager::getSprites()
	{
		return this->mSprites;
	}

	// Returns to a pointer to the sprite with the requested ID
	Sprite *SpriteManager::atID(int spriteID)
	{
		auto sprite = mSprites.find(spriteID);
		return sprite != mSprites.end() ? sprite->second : nullptr;
	}

	using json = nlohmann::json;
	void SpriteManager::parse(json j)
	{
		auto jSpriteList = j.get<std::vector<json>>();
		for (json& sj : jSpriteList)
		{
			createSprite(
				sj.at("SpriteName").get<std::string>(),
				sj.at("FileName").get<std::string>(),
				sj.at("SpriteID").get<int>()
			);
		}
	}

	json SpriteManager::serialize() {
		std::vector<json> jSpriteList;
		for (auto [id, sprite] : mSprites) {
			jSpriteList.push_back(sprite->serialize());
		}
		return jSpriteList;
	}

	// SPRITESHEET STUFF

	int SpriteManager::createSpriteSheet(std::string name, std::string filename)
	{
		unsigned int id = mCurrSpriteSheetID;

		// This will loop as long as it needs to until it finds the closest valid ID
		while (true)
		{
			// Check if sprite with id does not already exist
			if (mSpriteSheets[id] == nullptr)
			{
				// init id for sprite
				SpriteSheet *newSpriteSheet = new SpriteSheet(name, filename, id);
				this->mSpriteSheets[id] = newSpriteSheet;
				mCurrSpriteSheetID = id + 1;
				return id; // Return ID of the new sprite
			}
			// Otherwise, the specified sprite ID already exists, so check the next ID
			else
			{
				//printf("ID %d already used :( Trying SpriteSheetID: %d\n", id, id + 1);
				id++;
			}
		}
	}

	// This will overwrite the given id with a new spritesheet
	int SpriteManager::createSpriteSheet(std::string name, std::string filename, int id)
	{
		// init id for sprite
		SpriteSheet *newSpriteSheet = new SpriteSheet(name, filename, id);
		this->mSpriteSheets[id] = newSpriteSheet;
		return id; // Return ID of the new sprite
	}
	void SpriteManager::deleteSpriteSheet(int spritesheetID)
	{
		delete this->mSpriteSheets[spritesheetID];
		mSpriteSheets.erase(spritesheetID);
	}
	std::unordered_map<int, SpriteSheet *> &SpriteManager::getSpriteSheets()
	{
		return this->mSpriteSheets;
	}
	SpriteSheet *SpriteManager::atSheetID(int spritesheetID)
	{
		return mSpriteSheets[spritesheetID];
	}

	unsigned int SpriteManager::createPartialSprite(std::string name, int spriteID, SpriteSheet *spritesheet, glm::ivec2 location, glm::ivec2 dimensions)
	{
		// init id for sprite
		int id = spriteID;
		if (id == -1)
		{
			// Find next available id starting at 0
			id = 0;
			// This will loop as long as it needs to until it finds the closest valid ID
			while (true)
			{
				// Check if sprite with id does not already exist
				if (mSprites[id] == nullptr)
				{
					// init id for sprite
					mCurrSpriteID = id + 1;
					break;
				}
				// Otherwise, the specified sprite ID already exists, so check the next ID
				else
				{
					//printf("ID %d already used :( Trying SpriteID: %d\n", id, id + 1);
					id++;
				}
			}
		}

		// init name for sprite
		if (name == "")
		{
			name = spritesheet->getName();
		}

		Sprite *newSprite = new PartialSprite(name, id, spritesheet, location, dimensions);
		this->mSprites[id] = newSprite;
		return id; // Return ID of the new sprite
	}
	unsigned int SpriteManager::createLoopingSprite(std::string name, int spriteID, SpriteSheet *spritesheet, int numImages, float speed, glm::ivec2 loc, glm::ivec2 dims, int xpad)
	{
		// init id for sprite
		int id = spriteID;
		if (id == -1)
		{
			// Find next available id starting at 0
			id = 0;
			// This will loop as long as it needs to until it finds the closest valid ID
			while (true)
			{
				// Check if sprite with id does not already exist
				if (mSprites[id] == nullptr)
				{
					// init id for sprite
					mCurrSpriteID = id + 1;
					break;
				}
				// Otherwise, the specified sprite ID already exists, so check the next ID
				else
				{
					//printf("ID %d already used :( Trying SpriteID: %d\n", id, id + 1);
					id++;
				}
			}
		}

		// init name for sprite
		if (name == "")
		{
			name = spritesheet->getName();
		}

		Sprite *newSprite = new LoopingSprite(name, id, spritesheet, numImages, speed, loc, dims, xpad);
		this->mSprites[id] = newSprite;
		return id; // Return ID of the new sprite
	}
}