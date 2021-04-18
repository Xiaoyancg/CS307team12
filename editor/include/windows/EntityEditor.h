#pragma once

#include "windows/Window.h"
#include <string>

class EntityEditor : public Window
{
public:
    DEFAULT_CONSTRUCTOR(EntityEditor)
    {
        entityName[0] = '\0';
    }
    void draw();

private:
    char entityName[128];

    bool entityInfo;
    int x_pos;
    int y_pos;
    int entity_width = 0, entity_height = 0;
    int sprite_id;
};
