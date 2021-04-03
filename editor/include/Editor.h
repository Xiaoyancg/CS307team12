#pragma once
#include <cstdio>
#include <chrono>
#include <iostream>
#include <list>

#include <SDL.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imfilebrowser.h>

#include "EditorWindow.h"

#include "Game.h"
#include "UndoRedo.h"
#include "VMTool.h"
#ifdef __TEST_EDITOR
#include "TestEditor.h"
#include "Sprint1.h"
#endif // __TEST_EDITOR

enum SelectionEnum
{
	SAVEAS,
	DELETEPROJECT,
	GAMEVIEW,
	MAPVIEW,
	ENTITYEDITOR,
	PAGEEDITOR,
	MAPEDITOR,
	SAVEPOPUP,
	DELETEPOPUP,
	OBJECTTREE,
	SPRITEEDITOR,
	SCRIPTEDITOR,
	SPLASHSCREEN,

	// LEAVE THIS AT THE END OF THE LIST OR ELSE THIS IS A THREAT
	SELECT_COUNT,
};

enum CurrentComponent
{
	CUR_ENTITY,
	CUR_LOGIC,
	CUR_MAP,
	CUR_PAGE,
	CUR_SCRIPT,
	CUR_SPRITE,

	COMP_COUNT,
};

class Editor
{
public:
	Editor();

	void run();

	void initializeGraphics();
	void cleanupGraphics();
	void createWindows();
	void processInput();

	void ShowMainMenuBar();

	void markDeleteSuccess() {
		delete_success = true;
	}

	Core::Game *getGamePtr()
	{
		return game;
	}
	std::vector<EditorWindow*>& getWindowList() {
		return windowList;
	}
	std::vector<std::string>& getCurrentComponentList() {
		return currentComponent;
	}

	Core::Page *getCurrentPage()
	{
		return currPage;
	}
	Core::Map *getCurrentMap()
	{
		return currMap;
	}

	GLuint *getTexCBO()
	{
		return texcbo;
	}
	GLuint *getMapTexCBO()
	{
		return maptexcbo;
	}

private:
	Core::Game *game = nullptr;
	bool running = false;

	SDL_Window *sdlWindow = nullptr;
	SDL_GLContext gl_context;
	ImGuiIO *io;

	// WINDOWS
	std::vector<EditorWindow*> windowList;

	ImGui::FileBrowser saveDialog;
	ImGui::FileBrowser openDialog;
	ImGui::FileBrowser delDialog;
	ImGui::FileBrowser importDialog;

	// bool array to track the selections made on main menu bar
	bool selection[SELECT_COUNT];

    //deletion flag if an entity, map, or page is deleted
	bool delete_success = false;

	// main texture color buffer object
	// Game gets rendered onto this, and this is used as an Image with ImGUI
	GLuint *texcbo = nullptr;
	GLuint *maptexcbo = nullptr; // This is the framebuffer for the MapView
	// the bool used to enable/ disable control handler
	// check in game view

	// Current Map
	Core::Map *currMap = nullptr;

	// Current page pointer
	Core::Page *currPage = nullptr;

	// project directory
	std::string dir = "";

	// Game Name
	std::string gameName = "empty";

	// is game saved before (for save)
	bool isSaved = false;

	// is control key pressed
	bool ctrl_pressed = false;

	// currently selected game component
	std::vector<std::string> currentComponent;
	//std::string currentComponent = "No Component Selected";

	ImVec2 default_size = ImVec2(600, 400);
};
