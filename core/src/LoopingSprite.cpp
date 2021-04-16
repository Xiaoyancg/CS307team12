#include "LoopingSprite.h"

namespace Core {

    LoopingSprite::LoopingSprite(std::string name, int spriteID, SpriteSheet* spritesheet, int numImages, float speed, glm::ivec2 location, glm::ivec2 dimensions, int xpadding) :
        mSpriteSheet(spritesheet),
        mNumImages(numImages),
        mLocation(location),
        mDimensions(dimensions),
        mCurrImg(0),
        mPadding(xpadding)
    {
        mType = SPRITE_TYPES::LOOPING;
        mSpriteName = name;
        mSpriteID = spriteID;

        mImageSeconds = speed / numImages;
        mIsRunning = true;

        mTimer.stamp();

        updateTextureCoords();
    }

    unsigned int LoopingSprite::getOpenGLTextureID() {
        return mSpriteSheet->getOpenGLTextureID();
    }

    void LoopingSprite::setRunning(bool run) {
        mIsRunning = run;
    }

    bool LoopingSprite::isRunning() {
        return mIsRunning;
    }

    void LoopingSprite::updateTextureCoords() {
        if (mIsRunning) {
            if (mTimer.getPassedTimeFromStampInSec() >= mImageSeconds) {
                printf("%d time passed %f\n", mCurrImg, mTimer.getPassedTimeFromStampInSec());

                // Check if the current image is at the end of the loop, meaning the next 
                // image is at the beginning of the loop (wrap-around)
                if (mCurrImg == mNumImages - 1) {
                    mCurrImg = 0;
                }             
                else {
                    // Get next texture coords based on location/dims and current image
                    mCurrImg++;
                }

                glm::vec2 spritesheetDims(mSpriteSheet->getDimensions());
                glm::vec2 p1((mCurrImg * (mDimensions.x + mPadding)) + mLocation.x, mLocation.y + mDimensions.y);                   // Top left
                glm::vec2 p2((mCurrImg * (mDimensions.x + mPadding)) + mLocation.x, mLocation.y);                                   // Bottom left
                glm::vec2 p3((mCurrImg * (mDimensions.x + mPadding)) + mLocation.x + mDimensions.x, mLocation.y + mDimensions.y);   // Top right
                glm::vec2 p4((mCurrImg * (mDimensions.x + mPadding)) + mLocation.x + mDimensions.x, mLocation.y);                   // Bottom right
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

                mTimer.stamp();
            }
        }
    }
}