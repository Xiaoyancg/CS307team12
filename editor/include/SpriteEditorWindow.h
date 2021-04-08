#pragma once

#include "EditorWindow.h"
#include <imfilebrowser.h>


class SpriteEditorWindow : public EditorWindow {
public:
	DEFAULT_CONSTRUCTOR(SpriteEditorWindow)
	void draw();
private:
	ImGui::FileBrowser importDialog;
};
