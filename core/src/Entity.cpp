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
	float vertices[2] = {
		location.x, location.y
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	// Draw the bound buffer (vertices, for now)
	glPointSize(15);
	glDrawArrays(GL_POINTS, 0, 1);
}
