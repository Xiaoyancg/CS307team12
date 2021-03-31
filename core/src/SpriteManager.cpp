#include "SpriteManager.h"
#include <iostream>
namespace Core
{

	int SpriteManager::createSprite(std::string &spriteName,
									std::string &filename)
	{
		//// This will loop as long as it needs to until it finds the
		//// closest
		//// valid ID
		// Check if sprite with id does not already exist
		if (mSprites[spriteName] == nullptr)
		{
			// init id for sprite
			Sprite *newSprite = new Sprite(spriteName, filename);
			mSprites[spriteName] = newSprite;
			return 0; // Return ID of the new sprite
		}
		// Otherwise, the specified sprite ID already exists, so check the
		// next ID
		else
		{
			printf("name %s already used :(\n", spriteName);
			return 1;
		}
	}

	int SpriteManager::deleteSprite(std::string &spriteName_ref)
	{
		// check if there exist such sprite
		try
		{
			// free memory before erase
			delete mSprites.at(spriteName_ref);
			mSprites.erase(spriteName_ref);
			return 0;
		}
		// no such sprite
		catch (const std::exception &e)
		{
			// 1 for not found
			return 1;
		}
	}

	void SpriteManager::printSprites()
	{
		printf("SPRITES:\n");
		for (const auto &[key, value] : mSprites)
		{
			printf(" > ID: %d, %p\n", key, value);
		}
	}

	int SpriteManager::parse(nlohmann::json j)
	{
		try
		{
			for (nlohmann::json sj : j)
			{
				createSprite(
					sj.at("SpriteName").get<std::string>(),
					sj.at("FileName").get<std::string>());
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