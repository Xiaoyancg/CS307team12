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
	void drawExportPopup();
private:
	Editor* editor;

	bool deleteProjectPopup = false;
	bool projectDeletedPopup = false;
	bool saveAsPopup = false;

	ImGui::FileBrowser openDialog;
	ImGui::FileBrowser delDialog;
	ImGui::FileBrowser saveDialog;
	ImGui::FileBrowser exportDialog;
};
