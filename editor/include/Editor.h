#pragma once

#include <SDL.h>
#include <imgui.h>

#include "MainMenuBar.h"
#include "windows/Window.h"

#include "Game.h"
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
	void run();

	///////////////////////////////////////////
	// GUI HANDLER FUNCTIONS
	void initializeGraphics();
	void initializeFramebuffer();
	void cleanupGraphics();
	void createWindows();
	void processInput();
	void drawPopups();

	void markDeleteSuccess() {
		ImGui::OpenPopup("Delete Successful");
	}

	///////////////////////////////////////////
	// GAME HANDLER FUNCTIONS
	Core::Game *getGamePtr()
	{
		return game;
	}
	std::string& getGameFilePath() {
		return gameFilePath;
	}
	void createGame();
	void loadGame(const std::string filePath);
	void saveGame();
	void saveGameAs(const std::string filePath);
	void freeGame();

	std::vector<Window*>& getWindowList() {
		return windowList;
	}
	std::vector<std::string>& getCurrentComponentList() {
		return currentComponent;
	}

	Core::Map* getCurrentMap() {
		return currentMap;
	}

	void setCurrentMap(Core::Map* value) {
		currentMap = value;
	}

private:
	bool running = false;

	Core::Game *game = nullptr;
	std::string gameFilePath;

	SDL_Window *sdlWindow = nullptr;
	SDL_GLContext gl_context;
	ImGuiIO *io;

	MainMenuBar* mainMenuBar;
	// WINDOWS
	std::vector<Window*> windowList;

	Core::Map* currentMap;

	// main texture color buffer object
	// Game gets rendered onto this, and this is used as an Image with ImGUI
	GLuint mFBO = 0;
	GLuint mTexCBO = 0;
	// the bool used to enable/ disable control handler
	// check in game view

	// is control key pressed
	bool ctrl_pressed = false;

	// currently selected game component
	std::vector<std::string> currentComponent;
	//std::string currentComponent = "No Component Selected";

	ImVec2 default_size = ImVec2(600, 400);
};
