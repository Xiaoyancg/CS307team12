#pragma once

#include "windows/Window.h"

class LogicEditor : public Window
{
public:
    DEFAULT_CONSTRUCTOR(LogicEditor) {}
    void draw();

private:
    bool logicInfo = false;
    char keyCodeString[20] = "";
    SDL_Keycode keyCode;
    Uint32 keyType = 0;
};
