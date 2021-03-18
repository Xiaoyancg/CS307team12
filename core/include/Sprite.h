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

		unsigned char* getImageData();

	private:
		// Loads image in 'filename' into mImageData 
		void loadImageFromFilename(std::string filename);

		// This is where the image data will be stored (PAR-159)
		// Feel free to change the type whenever this is implemented if needed
		unsigned char* mImageData;

		glm::vec2 mImageDimensions;
	};
}