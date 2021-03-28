#pragma once
#include "Sprite.h"
#include <SOIL.h>
#include <glad/glad.h>

namespace Core
{
	Sprite::Sprite(std::string name, std::string filename)
	{
		mSpriteName = name;
		mTextureID = -1;

		int width;
		int height;
		int channels;

		// Load image into mImageData
		unsigned char *mImageData = SOIL_load_image(filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
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
			glTexImage2D(GL_TEXTURE_2D, 0, channelType, mImageDimensions.x, mImageDimensions.x, 0, channelType, GL_UNSIGNED_BYTE, mImageData);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the new sprite
			SOIL_free_image_data(mImageData); // Delete image data after loading into OpenGL
		}
		else
		{
			printf("Sprite::Sprite > Image loading error!\n");
		}
	}

	unsigned int Sprite::getOpenGLTextureID()
	{
		return mTextureID;
	}

	glm::vec2 Sprite::getDimensions()
	{
		return mImageDimensions;
	}

	std::string Sprite::getName()
	{
		return mSpriteName;
	}
}