#pragma once

#include "EditorWindow.h"


class MapWindow : public EditorWindow {
public:
    MapWindow(Editor* editor, ImVec2 size) : EditorWindow(editor, size) {}
    void draw();
};