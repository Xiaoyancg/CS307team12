#pragma once
#include "Sprite.h"
#include "FullSprite.h" // Neeeded for SpriteSheets
#include <glm/glm.hpp>

namespace Core
{
    // PartailSprite class
    class PartialSprite : public Sprite
    {
    public:
        // Needs spritesheet holding the sprite, its location, and its dimensions
        PartialSprite(std::string name, int spriteID, SpriteSheet* spritesheet, glm::ivec2 location, glm::ivec2 dimensions);
        unsigned int getOpenGLTextureID();

    private:
        // The SpriteSheet that contains the sprite loop. The sprite loop is found using the given location and dimensions
        SpriteSheet* mSpriteSheet;

        // The location on the image of the bottom left of your loop. This and the dimensions specify the "subset" of the spritesheet that consists of the image
        glm::ivec2 mLocation;

        // The width/height of each sprite in the sprite loop
        glm::ivec2 mDimensions;
    };
}