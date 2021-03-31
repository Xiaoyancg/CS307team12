#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>


// This class is for rendering text (needed on the MenuPage, maybe other things later)

namespace Core
{
    // Font class
    class Font
    {
    public:
        Font();

        // Render the given text with the specified location and size
        void renderText(std::string text, glm::ivec2 pos, int size, glm::vec3 color);

    private:
        // Source for the vertex shader
        const char* vertexSource = R"glsl(
            #version 330 core
            layout (location = 0) in vec4 pos;

            out vec2 TexCoords;

		    uniform vec2 scale; // This will scale our coordinates in pixels (0 < x,y < width,height) to opengl coordinates (-1 < x,y < 1)

            void main()
            {
                gl_Position = vec4(scale * pos.xy - 1, 0.0, 1.0);
                TexCoords = pos.zw;
            }  
	    )glsl";

        // Source for the fragment shader
        const char* fragmentSource = R"glsl(
            #version 330 core
            in vec2 TexCoords;
            out vec4 color;

            uniform sampler2D text;
            uniform vec3 textColor;

            void main()
            {    
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                color = vec4(textColor, 1.0) * sampled;
            }  
	    )glsl";


        struct Character {
            unsigned int TextureID;  // ID handle of the glyph texture
            glm::ivec2   Size;       // Size of glyph
            glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
            unsigned int Advance;    // Offset to advance to next glyph
        };

        // This maps holds the imported information for each character in the .ttf font file
        std::map<char, Character> mCharacters;

        // These are for rendering purposes
        inline static unsigned int VAO, VBO;
        inline static unsigned int textShaderProgram = -1;
    };
}