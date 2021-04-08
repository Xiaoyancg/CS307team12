#pragma once

#include "windows/Window.h"
#include <imfilebrowser.h>


class SpriteEditor : public Window {
public:
	DEFAULT_CONSTRUCTOR(SpriteEditor)
	void draw();
private:
	ImGui::FileBrowser importDialog;
};
