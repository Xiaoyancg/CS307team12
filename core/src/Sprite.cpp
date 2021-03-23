#include "Sprite.h"
#include <SOIL.h>
#include <glad/glad.h>

namespace Core
{
	Sprite::Sprite(std::string filename) {
		int width;
		int height;
		int channels;

		// Load image into mImageData
		unsigned char* mImageData = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
		mImageDimensions = glm::vec2(width, height);

		if (mImageData) {
			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);
			// Load mImageData into the currently bound texture (mTextureID)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mImageDimensions.x, mImageDimensions.x, 0, GL_RGB, GL_UNSIGNED_BYTE, mImageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(mImageData); // Delete image data after loading into OpenGL 
		}
		else {
			printf("Sprite::Sprite > Image loading error!\n");
		}
	}

	Sprite::~Sprite() {
	}

	unsigned int Sprite::getSprite() {
		return mTextureID;
	}

	glm::vec2 Sprite::getDimensions() {
		return mImageDimensions;
	}

}