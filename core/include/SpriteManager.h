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
		//// Constructor just sets mCurrSpriteID to the initial value of 0
		// empty constructor
		SpriteManager() {}

		/// \brief delete every sprite pointer
		~SpriteManager()
		{
			for (auto spritepair : mSprites)
				delete spritepair.second;
		}

		// Creates a new sprite based on the filename and returns the new
		// If the requested 'id' is already taken, or mCurrSpriteID has already
		// been taken, then the next closest ID will be found and used

		/// \brief Creates a new sprite at the correct location in mSprites
		/// \param spriteName
		/// \param filename
		/// \return int 0 if success, 1 if the spriteName was taken.
		int createSprite(std::string &spriteName_ref, std::string &fileName_ref);

		/// \brief Delete the sprite with the given ID
		/// \param spriteName_ref
		/// \return int 0 if success, 1 if doesn't exist sprite with the name
		int deleteSprite(std::string &spriteName_ref);

		// Unused for now, can be uncommented if needed
		// This may never be used, but if you need the whole list of sprites
		// here it is!
		// This might be used to iterate through all sprites,
		// but be warned that iteration with maps is in no specific order
		// Leaving it commented until a use case for it comes up
		std::unordered_map<std::string, Sprite *> getSprites()
		{
			return mSprites;
		}

		// Returns a pointer to the sprite with the requested ID
		// (in constant time because we're using unordered_maps instead of vectors!)
		Sprite *atName(std::string &spriteName) { return mSprites[spriteName]; }

		int getNumSprites() { return mSprites.size(); }

		// parse json
		int parse(nlohmann::json);

	private:
		// This is where sprites are stored, like an array based on their IDs
		// If a sprite has ID=32, then a pointer to it is stored at mSprites[32]
		std::unordered_map<std::string, Sprite *> mSprites;

		// Private function used for debugging
		void printSprites();
	};
}