#pragma once
#include "Sprite.h"
#include "FullSprite.h" // Needed for SpriteSheets
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
        LoopingSprite(std::string name, int spriteID, SpriteSheet* spritesheet, int numImages, glm::ivec2 location, glm::ivec2 dimensions);
        unsigned int getOpenGLTextureID();

    private:
        // The SpriteSheet that contains the sprite loop. The sprite loop is found using the given location and dimensions
        SpriteSheet* mSpriteSheet;

        // The number of images in the specified loop to loop. 
        int mNumImages;

        // The location on the image of the bottom left of your loop. This and the dimensions specify the "subset" of the spritesheet that consists of the image
        glm::ivec2 mLocation;

        // The width/height of each sprite in the sprite loop
        glm::ivec2 mDimensions;
    };
}