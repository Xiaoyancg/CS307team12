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

    Core::Entity *getCurrentEntity()
    {
        auto &entityList = editor->getGamePtr()->getCurrPage()->getEntityList();
        return currentEntityIdx >= 0 && currentEntityIdx < entityList.size() ? entityList[currentEntityIdx] : nullptr;
    }

private:
    char entityName[128];
    int entity_id = -1;
    int currentEntityIdx = -1;
    int x_pos = 0;
    int y_pos = 0;
    int entity_width = 0, entity_height = 0;
    int sprite_id = -1;
    int inScriptId = -1;
    bool invisible = true;
};
