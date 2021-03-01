#include "Page.h"
#include <glad/glad.h>
#include <stdio.h>

// Constructor of the game class
// Sets mWindow and sets/creates mPageContext
Page::Page(SDL_Window* window)
: mWindow(window) {
	mPageContext = SDL_GL_CreateContext(mWindow);
	if (!mPageContext) {
		perror("Error creating context\n");
	}
}

int Page::render() {
	// Show the current context
	SDL_GL_MakeCurrent(mWindow, mPageContext);

	// Clear the buffer with a blue background
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mWindow); // Show the buffer by bringing it to the front
	SDL_Delay(1000); // Wait 1 sec before continuing

	// Clear the buffer with a dark blue background
	glClearColor(0.0, 0.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mWindow); // Show the buffer by bringing it to the front
	SDL_Delay(1000); // Wait 1 sec before continuing


	return 0;
}