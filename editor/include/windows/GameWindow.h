#pragma once

#include "windows/Window.h"
#include <chrono>

class GameWindow : public Window {
public:
    GameWindow(Editor* editor, ImVec2 size, unsigned int texcbo, unsigned int fbo)
        : Window(editor, size), mTexCBO(texcbo), mFBO(fbo)
    {
        glBindTexture(GL_TEXTURE_2D, mTexCBO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, editor->getGamePtr()->width, editor->getGamePtr()->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        updateLastTime();
    }
    void draw();
    void handleClick(float, float, float, float);
    void handleMenuPageClick(glm::vec2);
    void handleMapPageClick(glm::vec2);
    void updateLastTime() {
        lastTime = std::chrono::steady_clock::now();
    }
private:
    unsigned int mTexCBO;
    unsigned int mFBO;
    std::chrono::steady_clock::time_point lastTime;
};
