#include "LoopingSprite.h"

namespace Core {

    LoopingSprite::LoopingSprite(std::string name, int spriteID, SpriteSheet* spritesheet, int numImages, glm::ivec2 location, glm::ivec2 dimensions) :
        mSpriteSheet(spritesheet),
        mNumImages(numImages),
        mLocation(location),
        mDimensions(dimensions)
    {
        mType = SPRITE_TYPES::LOOPING;
        mSpriteName = name;
        mSpriteID = spriteID;
    
    }

    unsigned int LoopingSprite::getOpenGLTextureID() {
        return -123456;
    }

}