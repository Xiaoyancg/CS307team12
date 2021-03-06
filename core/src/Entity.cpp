#include "Entity.h"
#include <glad/glad.h>

extern int width;
extern int height;

// Constructor of the game class
Entity::Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID)
: location(location),
scale(scale),
rotation(rotation), 
spriteID(spriteID) {
	printf("w: %d\n", width);
	printf("h: %d\n", height);

	float halfWidthPixels = width / 2;
	float halfHeightPixels = height / 2;
	float halfScaleWidthPixels = scale.x / 2;
	float halfScaleHeightPixels = scale.y / 2;
	float halfScaleWidthInCoords = halfScaleWidthPixels / halfWidthPixels;
	float halfScaleHeightInCoords = halfScaleHeightPixels / halfHeightPixels;

	coords[0] = location.x - halfScaleWidthInCoords; // Top left x
	coords[1] = location.y + halfScaleHeightInCoords; // Top left y
	printf("(%f, %f)", coords[0], coords[1]);

	coords[2] = location.x - halfScaleWidthInCoords; // Bottom left x
	coords[3] = location.y - halfScaleHeightInCoords; // Bottom left y
	printf("(%f, %f)", coords[2], coords[3]);

	coords[4] = location.x + halfScaleWidthInCoords; // Top right x
	coords[5] = location.y + halfScaleHeightInCoords; // Top right y
	printf("(%f, %f)", coords[4], coords[5]);

	coords[6] = location.x + halfScaleWidthInCoords; // Bottom right x
	coords[7] = location.y - halfScaleHeightInCoords; // Bottom right y
	printf("(%f, %f)", coords[6], coords[7]);

	printf("\n");
}


void Entity::render() {



	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

	// Draw the bound buffer (vertices, for now)
	//glPointSize(5);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDrawArrays(GL_POINTS, 0, 4);

}
