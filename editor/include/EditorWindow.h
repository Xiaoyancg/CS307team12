#pragma once

#include <imgui.h>
class Editor;


class EditorWindow {
public:
    EditorWindow(Editor* editor, ImVec2 size) : editor(editor), size(size) {}

    virtual void draw() = 0;

    bool isVisible() {
        return visible;
    }
    void setVisible(bool value) {
        visible = value;
    }
    bool* getVisiblePtr() {
        return &visible;
    }

    bool isFocused() {
        return focused;
    }

protected:
    Editor *editor;
    ImVec2 size;

    bool visible = false;
    bool focused = false;

    void updateWindowFocus() {
        focused = ImGui::IsWindowFocused(0);
    }
};
