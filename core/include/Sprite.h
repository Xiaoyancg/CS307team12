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
		~Sprite();

		unsigned char* getImageData();

		glm::vec2 getDimensions();

	private:
		// Loads image in 'filename' into mImageData 
		bool loadImageFromFilename(std::string filename);

		unsigned char* mImageData;

		glm::vec2 mImageDimensions;
	};
}