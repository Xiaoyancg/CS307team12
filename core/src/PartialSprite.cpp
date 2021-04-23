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

        // Get texture coords based on location/dims
        glm::vec2 spritesheetDims(spritesheet->getDimensions());
        glm::vec2 p1(location.x, location.y + dimensions.y);                   // Top left
        glm::vec2 p2(location.x, location.y);                                  // Bottom left
        glm::vec2 p3(location.x + dimensions.x, location.y + dimensions.y);    // Top right
        glm::vec2 p4(location.x + dimensions.x, location.y);                   // Bottom right
        p1 = glm::vec2(p1.x / spritesheetDims.x, p1.y / spritesheetDims.y);
        p2 = glm::vec2(p2.x / spritesheetDims.x, p2.y / spritesheetDims.y);
        p3 = glm::vec2(p3.x / spritesheetDims.x, p3.y / spritesheetDims.y);
        p4 = glm::vec2(p4.x / spritesheetDims.x, p4.y / spritesheetDims.y);

        // Set texture coordinates
        mTextureCoordinates[0] = p1.x;
        mTextureCoordinates[1] = p1.y;
        mTextureCoordinates[2] = p2.x;
        mTextureCoordinates[3] = p2.y;
        mTextureCoordinates[4] = p3.x;
        mTextureCoordinates[5] = p3.y;
        mTextureCoordinates[6] = p4.x;
        mTextureCoordinates[7] = p4.y;

    }

    unsigned int PartialSprite::getOpenGLTextureID() {
        return mSpriteSheet->getOpenGLTextureID();
    }

    SpriteSheet* PartialSprite::getSpriteSheet() {
        return mSpriteSheet;
    }
}