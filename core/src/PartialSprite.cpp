#include "PartialSprite.h"

namespace Core {
    PartialSprite::PartialSprite(SpriteSheet* spritesheet, glm::ivec2 location, glm::ivec2 dimensions) :
        mSpriteSheet(spritesheet),
        mLocation(location),
        mDimensions(dimensions)
    { }

    int PartialSprite::getSpriteID() {
        return -123456;
    }

    unsigned int PartialSprite::getOpenGLTextureID() {
        return -123456;
    }

}