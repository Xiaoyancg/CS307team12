#include "Entity.h"

// Constructor of the game class
Entity::Entity(glm::vec2 location, double rotation, double scale, int spriteID)
: location(location),
rotation(rotation),
scale(scale),
spriteID(spriteID) {
	// ^ That's all this really needs to do right now i think
}