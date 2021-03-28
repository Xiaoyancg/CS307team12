#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Core
{
	// Sprite class
	class Sprite
	{
	public:
		Sprite(std::string name, std::string filename, int spriteID);

		Sprite(std::string name, std::string filename);

		unsigned int getOpenGLTextureID();

		glm::vec2 getDimensions();

		std::string getName();
		std::string getFileName();
		int getSpriteID();

	private:
		std::string mSpriteName;
		std::string mFileName;
		glm::vec2 mImageDimensions;
		int mSpriteID;

		unsigned int mTextureID; // Set by OpenGL
	};
}