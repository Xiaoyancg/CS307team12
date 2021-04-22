#include "Font.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"


// NOTE: Much of this code is from learnopengl.com/In-Practice/Text-Rendering, but repurposed
//        into the Font class to better suit our needs

namespace Core {

    Font::Font(std::string filepath) {
        // Init FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            printf("ERROR::FREETYPE: Could not init FreeType Library\n");
            return;
        }

        // Load font from file
        // This is path is temporary
        FT_Face face;
        if (FT_New_Face(ft, filepath.c_str(), 0, &face))
        {
            printf("ERROR::FREETYPE: Failed to load font\n");
            return;
        }

        // Set font size to 48
        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

                                               // Load first 128 ASCII characters from .ttf into memory
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                printf("ERROR::FREETYTPE: Failed to load Glyph/character %c from file", c);
                continue;
            }

            // generate texture for current ASCII character
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            mCharacters.insert(std::pair<char, Character>(c, character));
        }

        // Delete face and ft now that all characters have been loaded into mCharacters
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // If the shaders haven't been setup already (meaning this is the first Font object created)
        if (Font::textShaderProgram == -1) {
            // okay...
            // now all characters are ready to be used, but we need a fancy shader for rendering text.
            // This chunk of code is just to set up the shaders
            int prevVAO, prevVBO;
            glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prevVBO);

            glGenVertexArrays(1, &Font::VAO);
            glGenBuffers(1, &Font::VBO);
            glBindVertexArray(Font::VAO);
            glBindBuffer(GL_ARRAY_BUFFER, Font::VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(int) * 16, NULL, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(0); // Enable attribute 0
            glEnableVertexAttribArray(1); // Enable attribute 1


            // Needed to clean up background of each character
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


            // Create vertex and fragment shaders
            GLuint vertexShaderID, fragmentShaderID;
            vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

            // Set the source code of the shaders
            glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
            glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);

            // Debug variables
            int success;
            char infoLog[512];

            // Compile the vertex shader, or print an error
            glCompileShader(vertexShaderID);
            glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
                printf("ERROR::TEXTSHADER::VERTEX::COMPILATION_FAILED\n>%s\n", infoLog);
            };

            // Compile the fragment shader, or print an error
            glCompileShader(fragmentShaderID);
            glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
                printf("ERROR::TEXTSHADER::FRAGMENT::COMPILATION_FAILED\n>%s\n", infoLog);
            };

            // Create new shader program
            Font::textShaderProgram = glCreateProgram();

            // Attach shaders to the new shader program
            glAttachShader(Font::textShaderProgram, vertexShaderID);
            glAttachShader(Font::textShaderProgram, fragmentShaderID);

            // Links the attached shaders in the program to their correct shader processors
            glLinkProgram(Font::textShaderProgram);

            // Delete the created shaders, now that they have already been linked
            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);


            // Reset VAO and VBO
            glBindVertexArray(prevVAO);
            glBindBuffer(GL_ARRAY_BUFFER, prevVBO);


        }
    }

    int Font::renderText(std::string text, glm::vec2 pos, float size, glm::vec3 color) {
        // Preserve previous opengl bindings
        int prevVAO, prevVBO, prevShader;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prevVBO);
        glGetIntegerv(GL_CURRENT_PROGRAM, &prevShader);

        // Use text shader
        glUseProgram(Font::textShaderProgram);

        glUniform1i(glGetUniformLocation(Font::textShaderProgram, "text"), 0); // Set texture uniform
        glUniform3f(glGetUniformLocation(Font::textShaderProgram, "textColor"), color.x, color.y, color.z); // Set given color

        glm::mat4 projection = glm::ortho(-Core::Game::width / 2.0f, Core::Game::width / 2.0f, -Core::Game::height / 2.0f, Core::Game::height / 2.0f);
        int camera = glGetUniformLocation(Font::textShaderProgram, "projection");
        glUniformMatrix4fv(camera, 1, GL_FALSE, glm::value_ptr(projection));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(Font::VAO);
        glBindBuffer(GL_ARRAY_BUFFER, Font::VBO);

        int minX = pos.x;
        int maxX = 0;

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = mCharacters[*c];

            float xpos = pos.x + ch.Bearing.x * size;
            float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * size;

            float w = ch.Size.x * size;
            float h = ch.Size.y * size;

            // update VBO for each character
            float vertices[4][4] = {
                { xpos,     ypos,       0, 1 },
                { xpos,     ypos + h,   0, 0 },
                { xpos + w, ypos,       1, 1 },
                { xpos + w, ypos + h,   1, 0 }

            };

            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);

            glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

            // render quad
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            pos.x += (ch.Advance >> 6) * size; // bitshift by 6 to get value in pixels (2^6 = 64)

            maxX = xpos + w;
        }

        // Restore changed OpenGL bindings
        glBindVertexArray(prevVAO);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, prevVBO);
        glUseProgram(prevShader);

        return maxX - minX;
    }

    int Font::calcTextWidth(std::string text, float size) {
        int textWidth = 0;
        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = mCharacters[*c];
            int w = ch.Size.x * size;
            //textWidth += w;
            //textWidth += (ch.Bearing.x * size) + w;
            textWidth += (ch.Advance >> 6) * size;
        }
        return textWidth;
    }

    int Font::calcTextHeight(std::string text, float size) {
        int maxHeight = 0;

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = mCharacters[*c];
            int h = ch.Size.y * size;

            maxHeight = h > maxHeight ? h : maxHeight;
        }
        return maxHeight;
    }
}