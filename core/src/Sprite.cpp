#include "Sprite.h"
#include <SOIL.h>

namespace Core
{
	Sprite::Sprite(std::string filename) {
		loadImageFromFilename(filename);
	}

	Sprite::~Sprite() {
		SOIL_free_image_data(mImageData);
	}

	unsigned char* Sprite::getImageData() {
		return mImageData;
	}

	glm::vec2 Sprite::getDimensions() {
		return mImageDimensions;
	}

	bool Sprite::loadImageFromFilename(std::string filename) {
		int width;
		int height;
		int channels;
		mImageData = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
		mImageDimensions = glm::vec2(width, height);
		return mImageData != nullptr;
	}
}