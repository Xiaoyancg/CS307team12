#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Core
{
	// Sprite class
	class Sprite
	{
	public:
		Sprite(std::string name, std::string filename);

		unsigned int getOpenGLTextureID() { return mTextureID; }
		glm::vec2 getDimensions() { return mImageDimensions; }
		std::string getName() { return mSpriteName; }
		std::string getFileName() { return mFileName; }

	private:
		std::string mSpriteName;
		std::string mFileName;
		glm::vec2 mImageDimensions;

		// Set by OpenGL
		unsigned int mTextureID;
	};
}