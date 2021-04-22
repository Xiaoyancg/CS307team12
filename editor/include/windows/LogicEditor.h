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
    int keyTypeIdx = 0;
    Uint32 keyType = 0;
    std::vector<int> targetScriptList;
    int newTargetScript = 0;
    char *signal_type_strings[3] = {"Custom", "Key", "Collide"};
    // also the signal type
    int signal_type_idx = 0;
    char *script_type_strings[3] = {"Custom", "MoveConstantly", "Bounce"};
    // also the script type
    int script_type_idx = 0;
    Core::Logic *current_logic = nullptr;
    char logic_name[128] = "";
    int logicIDInput = -1;
    int current_logic_idx = 0;
};
