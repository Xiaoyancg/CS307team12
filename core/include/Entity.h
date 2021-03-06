#include <glm/glm.hpp>


// Entity class
class Entity {
public:
	// Constructor, just creates the context of the page
	// 'location' is the (x, y) coordinate of the Entity
	// 'scale' is the (width, height) of the Entity in pixels
	// 'rotation' is the rotation of the entity
	// spriteID is unimplemented, but will reference a specific loaded sprite
	Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID);

	// Render the given entity on the current context
	// Assumes the shaders have already been setup
	void render();

private:
	// Basic Entity variables
	glm::vec2 location; // Center of the entity (x, y)
	glm::vec2 scale; // (x width, y height)
	double rotation;
	float coords[8]; // Stores 4 pairs of (x, y) float coordinates, one for each corner of the entity
	
	// Sprite ID used to index sprites loaded into memory from the Game class
	// This isn't used yet though, I'm just adding it for later
	int spriteID;
};