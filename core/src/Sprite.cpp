#include "Sprite.h"

namespace Core
{
	Sprite::Sprite(std::string filename) {
		loadImageFromFilename(filename);
		printf("Creating sprite from: %s\n", filename.c_str());
	}

	unsigned char* Sprite::getImageData() {
		return mImageData;
	}

	void Sprite::loadImageFromFilename(std::string filename) {
		// TODO (PAR-159) set mImageData

		// TODO Set Sprite::mImageDimensions here as well
	}
}