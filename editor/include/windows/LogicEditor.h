#pragma once

#include "windows/Window.h"

class LogicEditor : public Window {
public:
    DEFAULT_CONSTRUCTOR(LogicEditor) {}
    void draw();
private:
    bool logicInfo = false;
};
