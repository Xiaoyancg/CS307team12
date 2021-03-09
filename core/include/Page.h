#pragma once
#include <SDL.h>
#include <glad/glad.h>
#include <stdio.h>

// Page class
// Pages can be menu, settings, HUD, map, Cutscene, ...
// Each page contains its own context and render function
class Page {

public:
	// Constructor, just creates the context of the page
	Page(SDL_Window* window);

	// Render the given page on the current context
	virtual void render();

private:
	SDL_GLContext mPageContext; // The page context
	SDL_Window* mWindow; // The window the page context is related to
};