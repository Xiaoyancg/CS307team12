#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Core
{
    // SpriteHighlighter class
    class SpriteHighlighter
    {
    public:
        // This holds the information necessary to find a Sprite in a SpriteSheet. This object is used to highlight on a SpriteSheet in the Sprite editor
        SpriteHighlighter() : mNumImages(0), mLocation(0, 0), mDimensions(0, 0), mPadding(0) {};

        void setNumImages(int num) {
            mNumImages = num;
        }
        void setLocation(glm::ivec2 loc) {
            mLocation = loc;
        }
        void setDimensions(glm::ivec2 dim) {
            mDimensions = dim;
        }
        void setPadding(int pad) {
            mPadding = pad;
        }

        void highlight() {
            int x = mLocation.x;
            int y = mLocation.y;
            glLineWidth(1.0f);

            for (int i = 0; i < mNumImages; i++) {
                float verts[8];

                // Left side
                // P1
                verts[0] = x;  // Bottom left x
                verts[1] = y; // Bottom left y
                verts[2] = 0;
                verts[3] = 0;
                // P2
                verts[4] = x; // Top left x
                verts[5] = y + mDimensions.y; // Top left y
                verts[6] = 0;
                verts[7] = 0;
                glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verts, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, 2);

                // Top side
                // P1
                verts[0] = x + mDimensions.x;  // Top right x
                verts[1] = y + mDimensions.y; // Top right y
                verts[2] = 0;
                verts[3] = 0;
                glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verts, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, 2);

                // Right side
                // P2
                verts[4] = x + mDimensions.x; // Bottom right x
                verts[5] = y; // Bottom right y
                verts[6] = 0;
                verts[7] = 0;
                glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verts, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, 2);

                // Bottom side
                // P1
                verts[0] = x;  // Bottom left x
                verts[1] = y; // Bottom left y
                verts[2] = 0;
                verts[3] = 0;
                glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), verts, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, 2);

                x += mDimensions.x + mPadding;
            }
        }
    private:
        // The number of images in the specified loop to loop. 
        int mNumImages;

        // The location on the image of the bottom left of your loop. This and the dimensions specify the "subset" of the spritesheet that consists of the image
        glm::ivec2 mLocation;

        // The width/height of each sprite in the sprite loop
        glm::ivec2 mDimensions;

        // Set to xpadding from constructor. The distance in pixels between each sprite in the loop
        int mPadding; 
    };
}