#pragma once

#include "windows/Window.h"


class GameWindow : public Window {
public:
    GameWindow(Editor* editor, ImVec2 size, unsigned int texcbo, unsigned int fbo)
        : Window(editor, size), mTexCBO(texcbo), mFBO(fbo) {}
    void draw();
private:
    unsigned int mTexCBO;
    unsigned int mFBO;
};
