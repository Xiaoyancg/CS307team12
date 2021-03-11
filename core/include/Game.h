#pragma once
#include <SDL.h>
#include <Page.h>
#include <Entity.h>
#include <vector>
#include <memory> // For unique_ptr
#include <glad/glad.h>


class Game {
public:
	void sdl_die(const char* err_msg);
	int render();
	int coreMain(int argc, char* argv[]);

private:
	void initShaders();
	void init();


	SDL_Window* window; // Window of this Game
	SDL_GLContext gl_context; // The context of this Game
	unsigned int shaderProgram; // The shaders, set by initShaders before entering the game loop

	// This vector stores pointers to all of the entities
	// Vectors don't support normal pointers, like Entity *, so we use unique_ptr instead
	std::vector<std::unique_ptr<Entity>> entities;
};

