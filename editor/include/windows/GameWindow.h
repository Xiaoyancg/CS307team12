#pragma once

#include "windows/Window.h"


class GameWindow : public Window {
public:
    DEFAULT_CONSTRUCTOR(GameWindow)
    void draw();
};
