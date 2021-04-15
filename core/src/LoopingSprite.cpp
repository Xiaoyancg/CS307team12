#include "LoopingSprite.h"

namespace Core {

    LoopingSprite::LoopingSprite(SpriteSheet* spritesheet, int numImages, glm::ivec2 location, glm::ivec2 dimensions) :
        mSpriteSheet(spritesheet),
        mNumImages(numImages),
        mLocation(location),
        mDimensions(dimensions)
    { }

    int LoopingSprite::getSpriteID() {
        return -123456;
    }

    unsigned int LoopingSprite::getOpenGLTextureID() {
        return -123456;
    }

}