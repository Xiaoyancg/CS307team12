#pragma once
#include <SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include <Entity.h>
#include <string>
#include <vector>

namespace Core
{
// Page class
// Pages can be menu, settings, HUD, map, Cutscene, ...
// Each page contains its own context and render function
	class Page
	{

	public:
		// Constructor, just creates the context of the page
		Page ( SDL_Window *window );

		// Render the given page on the current context
		// Returns non-zero on error
		int SetName ( std::string name );
		std::string GetName ();
		std::vector<Entity> getEntityList ();
		virtual void render ();

	protected:
		SDL_GLContext mPageContext; // The page context
		SDL_Window *mWindow; // The window the page context is related to
		std::string name;
		std::vector<Entity> entityList;
	};
}