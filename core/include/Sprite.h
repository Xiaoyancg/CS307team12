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

		unsigned int getOpenGLTextureID();

		glm::vec2 getDimensions();

		std::string getName();

	private:
		std::string mSpriteName;

		glm::vec2 mImageDimensions;

		unsigned int mTextureID; // Set by OpenGL
	};
}