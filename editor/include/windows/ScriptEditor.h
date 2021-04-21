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
    void getInfo(Core::Script *);
    void listTreeNode(std::string, int);
    int currScriptIdx = -1;
    Core::Script *currScript_ptr = nullptr;
    char scriptName[128];
    bool scriptInfo = false;
    // info
    int scriptId = -1;
    int type = 0;
    const char *typeNameList[1] = {"Custom"}; //, "MoveConstantly"};
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
};