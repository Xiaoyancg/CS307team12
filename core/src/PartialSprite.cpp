#include "PartialSprite.h"

namespace Core {
    PartialSprite::PartialSprite(std::string name, int spriteID, SpriteSheet* spritesheet, glm::ivec2 location, glm::ivec2 dimensions) :
        mSpriteSheet(spritesheet),
        mLocation(location),
        mDimensions(dimensions)
    { 
        mType = SPRITE_TYPES::PARTIAL;
        mSpriteName = name;
        mSpriteID = spriteID;

        // TODO: Set texture coords based on location/dims
        glm::ivec2 spritesheetDims(spritesheet->getDimensions());
        glm::ivec2 p1(location.x, location.y + dimensions.y);                   // Top left
        glm::ivec2 p2(location.x, location.y);                                  // Bottom left
        glm::ivec2 p3(location.x + dimensions.x, location.y + dimensions.y);    // Top right
        glm::ivec2 p4(location.x + dimensions.x, location.y);                   // Bottom right
    }

    unsigned int PartialSprite::getOpenGLTextureID() {
        return -123456;
    }

}