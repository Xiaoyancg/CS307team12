#pragma once

#include "EditorWindow.h"


class GameWindow : public EditorWindow {
public:
    GameWindow(Editor* editor, ImVec2 size) : EditorWindow(editor, size) {}
    void draw();
};
