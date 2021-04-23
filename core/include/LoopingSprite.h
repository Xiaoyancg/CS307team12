#pragma once
#include "Sprite.h"
#include "FullSprite.h" // Needed for SpriteSheets
#include "Timer.h"
#include <glm/glm.hpp>

namespace Core
{
    // LoopingSprite class
    class LoopingSprite : public Sprite
    {
    public:
        // LoopingSprite needs a Sprite pointer (probably to a loaded SpriteSheet), the number of images to loop, 
        // the location of the sprite loop in the image (pixel location of the bottom left of the loop), and the w/h of each sprite in the loop.
        // The location is specified based on the distance from the bottom left of the screen
        LoopingSprite(std::string name, int spriteID, SpriteSheet* spritesheet, int numImages, float speed, glm::ivec2 location, glm::ivec2 dimensions, int xpadding);
        unsigned int getOpenGLTextureID();
        SpriteSheet* getSpriteSheet();

        // Argument will set mIsRunning to either start or stop sprite looping
        void setRunning(bool);
        bool isRunning();

        // This is used to move the sprite shown on the screen to the next in the spritesheet (or loop-around)
        // after a specified amount of time if necessary. It's called each time an 
        void updateTextureCoords(); 

    private:
        // The SpriteSheet that contains the sprite loop. The sprite loop is found using the given location and dimensions
        SpriteSheet* mSpriteSheet;

        // True means the sprite loop is running, false will stop the loop
        bool mIsRunning;

        // The number of images in the specified loop to loop. 
        int mNumImages;
        // The current image in the range [0, mNumImages] being displayed (used in texcoord calculation)
        int mCurrImg;

        // The speed of each image in the loop in seconds (used to check for timing in updateTextureCoords())
        float mImageSeconds; // TODO: Maybe turn into an array of size mNumImages, specify length of each individual image

        // The location on the image of the bottom left of your loop. This and the dimensions specify the "subset" of the spritesheet that consists of the image
        glm::ivec2 mLocation;

        Timer mTimer; // The timer is used with stamps to check 

        // The width/height of each sprite in the sprite loop
        glm::ivec2 mDimensions;
        int mPadding; // Set to xpadding from constructor
    };
}