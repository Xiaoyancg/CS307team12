#include <glm/glm.hpp>


// Entity class
class Entity {
public:
	// Constructor, just creates the context of the page
	Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID);

	// Render the given entity on the current context
	// Assumes the shaders have already been setup
	void render();

private:
	// Basic Entity variables
	glm::vec2 location;
	glm::vec2 scale;
	double rotation;
	
	// Sprite ID used to index sprites loaded into memory from the Game class
	// This isn't used yet though, I'm just adding it for later
	int spriteID;
};