#pragma once
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Sprite.h"
#include "FullSprite.h"
#include "PartialSprite.h"


// We can't predict OpenGL texture/sprite ID's, and we need some consistent way to reference specific sprites in the .gdata files,
// so my solution was to use a SpriteManager to translate between the Parchment Sprite IDs and their corresponding OpenGL texture IDs.

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
		// Constructor just sets mCurrSpriteID to the initial value of 0
		SpriteManager();

		// Creates a new sprite based on the filename and returns the new sprite's ID based on mCurrSpriteID or the specified 'id'.
		// If the requested 'id' is already taken, or mCurrSpriteID has already been taken, then the next closest ID will be found and used
		int createSprite(std::string name, std::string filename);
		int createSprite(std::string name, std::string filename, int id);

		// These do the same thing as createSprite, but to the mSpriteSheets map instead
		// We need the separate maps because spritesheets shouldn't be considered Sprites.
		// There was no need to create a separate class for SpriteSheets because all operations are the same as Sprite.cpp.
		int createSpriteSheet(std::string name, std::string filename);
		int createSpriteSheet(std::string name, std::string filename, int id);

		unsigned int createPartialSprite(std::string name, int spriteID, SpriteSheet* spritesheet, glm::ivec2 location, glm::ivec2 dimensions);

		// Delete the sprite with the given ID
		void deleteSprite(int spriteID);
		void deleteSpriteSheet(int spritesheetID);

		// Unused for now, can be uncommented if needed
		std::unordered_map<int, Sprite*>& getSprites();
		std::unordered_map<int, SpriteSheet*>& getSpriteSheets();

		// Returns a pointer to the sprite with the requested ID
		// (in constant time because we're using unordered_maps instead of vectors!)
		Sprite* atID(int spriteID);
		SpriteSheet* atSheetID(int spriteID);

		// parse json
		int parse(nlohmann::json);

	private:
		int mCurrSpriteID; // The current sprite ID, which will be set to whatever sprite is created next
		int mCurrSpriteSheetID; // The current sprite ID, which will be set to whatever sprite is created next

		// This is where sprites are stored, like an array based on their IDs
		// If a sprite has ID=32, then a pointer to it is stored at mSprites[32]
		std::unordered_map<int, Sprite*> mSprites;
		std::unordered_map<int, SpriteSheet*> mSpriteSheets;

		// Used for debugging
		void printSprites();
	};
}