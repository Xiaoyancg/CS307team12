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
	int currScriptId = 0;
	char scriptName[128];
	bool scriptInfo;
};