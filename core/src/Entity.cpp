#include "Entity.h"
#include <glad/glad.h>


// Constructor of the game class
Entity::Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID)
: location(location),
scale(scale),
rotation(rotation), 
spriteID(spriteID) {
	// ^ That's all this really needs to do right now i think
}


void Entity::render() {
	glClearColor(1.0, 0.0, 1.0, 1.0); // Pink color background
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the bound buffer (vertices)
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
