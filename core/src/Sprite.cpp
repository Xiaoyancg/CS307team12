#pragma once
#include "Sprite.h"
#include <SOIL.h>
#include <glad/glad.h>

namespace Core
{
	Sprite::Sprite(std::string spriteName, std::string filename)
	{
		mSpriteName = spriteName;
		mFileName = filename;
		// this will be 4294967295 since texture id is uint
		mTextureID = -1;

		int width;
		int height;
		int channels;

		// Load image into mImageData
		unsigned char *mImageData = SOIL_load_image(filename.c_str(),
													&width, &height,
													&channels, SOIL_LOAD_AUTO);
		mImageDimensions = glm::vec2(width, height);

		if (mImageData)
		{
			glGenTextures(1, &mTextureID);
			glBindTexture(GL_TEXTURE_2D, mTextureID);
			// set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			unsigned channelType;
			switch (channels)
			{
			case 3:
				channelType = GL_RGB;
				break;
			case 4:
				channelType = GL_RGBA;
				break;
			default:
				channelType = GL_RGB;
				break;
			}
			// Load mImageData into the currently bound texture (mTextureID)
			glTexImage2D(GL_TEXTURE_2D, 0, channelType,
						 mImageDimensions.x, mImageDimensions.x,
						 0, channelType, GL_UNSIGNED_BYTE, mImageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			// Unbind the new sprite
			glBindTexture(GL_TEXTURE_2D, 0);
			// Delete image data after loading into OpenGL
			SOIL_free_image_data(mImageData);
		}
		else
		{
			printf("Sprite::Sprite > Image loading error!\n");
		}
	}
}