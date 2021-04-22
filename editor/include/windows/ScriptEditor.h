#pragma once

#include "windows/Window.h"
#include <string>

class ScriptEditor : public Window
{
public:
    DEFAULT_CONSTRUCTOR(ScriptEditor)
    {
        scriptName[0] = '\0';
    }
    void draw();

private:
    void customWidgets();
    void MoveConstantlyWidgets();
    void BounceWidgets();
    void getInfo(Core::Script *);
    void customTargetListTreeNode(std::string, int);
    int currScriptIdx = -1;
    Core::Script *currScript_ptr = nullptr;
    bool scriptInfo = false;

    //* ------------------------ INFO ------------------------ *//
    char scriptName[128];
    int scriptId = -1;
    int type = 0;
    const char *typeNameList[3] = {"Custom", "MoveConstantly", "Bounce"};

    //* ----------------------- CUSTOM ----------------------- *//

    std::vector<int> addTargetSignalList;
    int newAddSignal = 0;
    std::vector<int> addTargetLogicList;
    int newAddLogic = 0;
    std::vector<int> addTargetScriptList;
    int newAddScript = 0;
    std::vector<int> removeTargetSignalList;
    int newRemoveSignal = 0;
    std::vector<int> removeTargetLogicList;
    int newRemoveLogic = 0;
    std::vector<int> removeTargetScriptList;
    int newRemoveScript = 0;

    //* ------------------- MOVE CONSTANTLY ------------------ *//
    int x = 0, y = 0;
    int targetPageId = -1;
    int targetEntityId = -1;
    std::vector<int> targetEntityList;
    int newEntity = 0;
};