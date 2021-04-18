#pragma once

#include "windows/Window.h"
#include <ctime>

class GameWindow : public Window {
public:
    GameWindow(Editor* editor, ImVec2 size, unsigned int texcbo, unsigned int fbo)
        : Window(editor, size), mTexCBO(texcbo), mFBO(fbo)
    {
        glBindTexture(GL_TEXTURE_2D, mTexCBO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, editor->getGamePtr()->width, editor->getGamePtr()->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void draw();
    void updateLastTime() {
        lastTime = std::clock();
        lastFrame = 0;
    }
private:
    unsigned int mTexCBO;
    unsigned int mFBO;
    long lastTime = 0;
    long lastFrame = 0;
};
