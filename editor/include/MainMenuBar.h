#pragma once

class Editor;
class MainMenuBar;
#include "Editor.h"
#include <imfilebrowser.h>


class MainMenuBar {
public:
	MainMenuBar(Editor* editor) : editor(editor) {}
	void draw();
	void drawOpenDialog();
	void drawDeleteProjectPopup();
	void drawSaveAsPopup();
private:
	Editor* editor;
	ImGui::FileBrowser openDialog;
	ImGui::FileBrowser delDialog;
	ImGui::FileBrowser saveDialog;
};
