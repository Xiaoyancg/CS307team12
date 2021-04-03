#pragma once

#include <string>
#include "EditorWindow.h"


class EntityEditorWindow : public EditorWindow {
public:
    DEFAULT_CONSTRUCTOR(EntityEditorWindow)
    void draw();
private:
    char entityName[128];

    bool entityInfo;
    int x_pos;
    int y_pos;
};
