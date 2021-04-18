#pragma once

#include "windows/Window.h"

class ScriptEditor : public Window
{
public:
	DEFAULT_CONSTRUCTOR(ScriptEditor) {}
	void draw();

private:
	int currScriptId = 0;
};
