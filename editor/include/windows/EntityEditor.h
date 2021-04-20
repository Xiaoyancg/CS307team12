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

    bool entityInfo = false;
    int x_pos = 0;
    int y_pos = 0;
    int entity_width = 0, entity_height = 0;
    int sprite_id = -1;
};
