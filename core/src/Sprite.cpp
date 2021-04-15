#pragma once
#include "Sprite.h"

namespace Core
{
	glm::vec2 Sprite::getDimensions()
	{
		return mImageDimensions;
	}

	std::string Sprite::getName()
	{
		return mSpriteName;
	}

	std::string Sprite::getFileName()
	{
		return this->mFileName;
	}
	int* Sprite::getTextureCoordinates() {
		return mTextureCoordinates;
	}

}