#pragma once

class Editor;
#include "Editor.h"


class MainMenuBar {
public:
	MainMenuBar(Editor* editor) : editor(editor) {}
	void draw();
private:
	Editor* editor;
};
