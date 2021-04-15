#include "Sprite.h"
#include <glm/glm.hpp>

namespace Core
{
    // FullSprite class
    // This is for sprites that take up an entire imported image
    class FullSprite : public Sprite
    {
    public:
        FullSprite(std::string name, std::string filename, int spriteID);
        unsigned int getOpenGLTextureID();
        int getSpriteID();

    private:
        unsigned int mTextureID; // Set by OpenGL
    };


    typedef FullSprite SpriteSheet;
}