#pragma once
#include <string>
#include <glm/glm.hpp>

/*
 * Sprite class is now just a base class for different types of Sprites. The different types of Sprites are created with the SpriteManager.
 */

namespace Core
{
	enum class SPRITE_TYPES {
		FULL,			// Display the entire loaded image
		PARTIAL,		// Display part of a loaded SpriteSheet
		LOOPING			// Display a looping sequence of a SpriteSheet
	};

	// Sprite class
	class Sprite
	{
	public:
		virtual unsigned int getOpenGLTextureID() = 0; // Make 'Sprite' pure-virtual, no 'Sprite' objects can exist, but derived Sprite classes can (like FullSprite, LoopingSprite, ...)

		int getSpriteID(); // Get Parchment ID for each sprite

		glm::vec2 getDimensions();

		std::string getName();
		std::string getFileName();

		float* getTextureCoordinates();

	protected:
		std::string mSpriteName;
		std::string mFileName;

		float mTextureCoordinates[8];

		glm::vec2 mImageDimensions;

		enum class SPRITE_TYPES mType; // Stores the type of the Sprite, set by its derived class

		// The Parchment ID of the Sprite
		int mSpriteID;
	};
}