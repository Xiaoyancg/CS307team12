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
	float* Sprite::getTextureCoordinates() {
		return mTextureCoordinates;
	}

	int Sprite::getSpriteID() {
		return mSpriteID;
	}

	enum class SPRITE_TYPES Sprite::getType() {
		return mType;
	}
}