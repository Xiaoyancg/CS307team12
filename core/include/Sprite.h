#pragma once
#include <string>
#include <glm/glm.hpp>


/*
 * Sprite class is now just a base class for different types of Sprites. The different types of Sprites are created with the SpriteManager.
 */

namespace Core
{
	extern class FullSprite; // Using extern here instead of importing FullSprite.h allows Sprite.h to compile before FullSprite, which relies on Sprite.h
	// SpriteSheets contain the same functionality as FullSprites, so there is no need to create a new Object for them
	typedef FullSprite SpriteSheet;

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
		virtual int getSpriteID() = 0; // Make 'Sprite' pure-virtual, no 'Sprite' objects can exist, but derived Sprite classes can (like FullSprite, LoopingSprite, ...)

		glm::vec2 getDimensions();

		std::string getName();
		std::string getFileName();

		int* getTextureCoordinates();

	protected:
		std::string mSpriteName;
		std::string mFileName;

		int mTextureCoordinates[8];

		glm::vec2 mImageDimensions;

		enum class SPRITE_TYPES mType; // Stores the type of the Sprite, set by its derived class
	};
}