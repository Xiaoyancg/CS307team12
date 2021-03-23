#pragma once
#include <string>
#include <glm/glm.hpp>


namespace Core
{
	// Sprite class
	class Sprite
	{
	public:
		Sprite(std::string filename);

		unsigned int getSprite();

		glm::vec2 getDimensions();

	private:
		glm::vec2 mImageDimensions;

		unsigned int mTextureID; // Set by OpenGL
	};
}