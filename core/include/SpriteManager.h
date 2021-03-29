#pragma once
#include <string>
#include <unordered_map>
#include <queue>

#include <nlohmann/json.hpp>
#include "Sprite.h"

// We can't predict OpenGL texture/sprite ID's, and we need some consistent way
// to reference specific sprites in the .gdata files, so my solution was to
// use a SpriteManager to translate between the Parchment Sprite IDs and their
// corresponding OpenGL texture IDs.

/*
 * Use the SpriteManager when using sprites
 *
 * This will handle constant-time access of sprites based on their ID 
 * ( using SpriteManager::atID(id) )
 * as well as set the each sprite ID automatically.
 */
namespace Core
{
	// SpriteManager class
	class SpriteManager
	{
	public:
		// Constructor just sets mCurrSpriteID to the initial value of 0
		SpriteManager() : mCurrSpriteID(0) {}

		// Creates a new sprite based on the filename and returns the new
		// sprite's ID based on mCurrSpriteID or the specified 'id'.
		// If the requested 'id' is already taken, or mCurrSpriteID has already
		// been taken, then the next closest ID will be found and used
		int createSprite(std::string name, std::string filename, int id);
		// this one is depreciated
		int createSprite(std::string name, std::string filename);

		// Delete the sprite with the given ID
		void deleteSprite(int spriteID);

		// Unused for now, can be uncommented if needed
		// This may never be used, but if you need the whole list of sprites
		// here it is!
		// This might be used to iterate through all sprites,
		// but be warned that iteration with maps is in no specific order
		// Leaving it commented until a use case for it comes up
		std::unordered_map<int, Sprite *> getSprites() { return this->mSprites; }

		// Returns a pointer to the sprite with the requested ID
		// (in constant time because we're using unordered_maps instead of vectors!)
		Sprite *atID(int spriteID) { return mSprites[spriteID]; }

		int getNumSprites() { return this->mnumSprites; }
		void setNumSprites(int numSprite) { this->mnumSprites = numSprite; }

		// parse json
		int parse(nlohmann::json);

		std::vector<int> getEmptyIDV() { return this->memptyIDV; }
		void pushEmptyIDV(int id) { this->memptyIDV.push_back(id); }

	private:
		// The current sprite ID,
		// which will be set to whatever sprite is created next
		int mCurrSpriteID;

		// stores the deleted id, will be used in create
		std::vector<int> memptyIDV;

		// total number of sprites
		int mnumSprites;

		// This is where sprites are stored, like an array based on their IDs
		// If a sprite has ID=32, then a pointer to it is stored at mSprites[32]
		std::unordered_map<int, Sprite *> mSprites;

		// Used for debugging
		void printSprites();
	};
}