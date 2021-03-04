#include <SDL.h>
#include <glm.hpp>

// Entity class

class Entity {

public:
	// Constructor, just creates the context of the page
	Entity(glm::vec2, double, double, int);

	// Render the given page on the current context
	// Returns non-zero on error
	int render();

private:
	// Basic Entity variables
	glm::vec2 location;
	double rotation;
	double scale;

	// Sprite ID used to index sprites loaded into memory from the Game class
	// This isn't used yet though, I'm just adding it for later
	int spriteID;
}