#include "Entity.h"

// Constructor of the game class
Entity::Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID)
: location(location),
scale(scale),
rotation(rotation), 
spriteID(spriteID) {
	// ^ That's all this really needs to do right now i think
}

int Entity::render() {

	return 0;
}