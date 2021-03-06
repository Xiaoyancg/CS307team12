#include "Entity.h"
#include <glad/glad.h>

extern int width; // Width of the window, set in Game.cpp
extern int height; // Height of the window, set in Game.cpp

// Constructor of the game class
Entity::Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID)
: location(location),
scale(scale),
rotation(rotation), 
spriteID(spriteID) {
	/*
	 * All this constructor does is calculate the coordinates of the 4 corners of the entity, based on location and scale
	 * 
	 * In order to calculate the corner coordinates of the entity, we add and subtract distances (which are calculated from 'scale') from 'location'
	 *
	 * The given scale is in pixels (0 < x < width, 0 < y < height) while the location is in opengl coordinates (-1 < x < 1, -1 < y < 1)
	 * 
	 * This constructor will convert the scale from pixels to distances based on the opengl coordinate system, and calculate the Entity coordinates
	 */

	// if the width is 300px, pixelX(0, 150) = oglX(-1, 0) and pixelX(150, 300) = oglX(0, 1)
	// By finding the half amounts, we can scale the number of pixels (0 < scale < width or height) to a distance based on the opengl coordinates (0 < distance < 1)
	float halfWidthPixels = width / 2;
	float halfHeightPixels = height / 2;

	// Get the half dimensions of the scale
	// Location denotes the center of the tile, so (scale.x / 2) pixels will be to the left and (scale.x / 2) pixels to the right
	// These variables store the number of pixels to be moved from the location in the left/right or up/down direction for each coordinate
	float halfScaleWidthPixels = scale.x / 2;
	float halfScaleHeightPixels = scale.y / 2;

	// Convert the number of pixels to be moved from the location into a distance based on opengl coordinates
	// These variables store the distance to be moved from the location in the left/right or up/down direciton
	float halfScaleWidthInCoords = halfScaleWidthPixels / halfWidthPixels;
	float halfScaleHeightInCoords = halfScaleHeightPixels / halfHeightPixels;

	// Entities are drawn using the GL_TRIANGLE_STRIP opengl primitive (in Entity::render()), which follows this pattern:
	// P1  P3   P5   ...  
	//  | / |  / | /
	// P2   P4  P6

	// Tiles only need 4 points, so 
	// P1  P3
	//  | / |
	// P2   P4

	// Calculate the left-most and right-most x coordinate, and the up-most and down-most y coordinate
	float lowX = location.x - halfScaleWidthInCoords;
	float highX = location.x + halfScaleWidthInCoords;
	float lowY = location.y - halfScaleHeightInCoords;
	float highY = location.y + halfScaleHeightInCoords;

	// Calculate P1, P2, P3, and P4 based on the calculated coordinates ^
	// P1
	coords[0] = lowX; // Top left x
	coords[1] = highY; // Top left y
	// P2
	coords[2] = lowX; // Bottom left x
	coords[3] = lowY; // Bottom left y
	// P3
	coords[4] = highX; // Top right x
	coords[5] = highY; // Top right y
	// P4
	coords[6] = highX; // Bottom right x
	coords[7] = lowY; // Bottom right y

	// Print out the corner coordinates of the entity
	//printf("(%f, %f)(%f, %f)(%f, %f)(%f, %f)\n", coords[0], coords[1], coords[2], coords[3], coords[4], coords[5], coords[6], coords[7]);
}


void Entity::render() {
	// Load the data of the 'coords' buffer into the currently bound array buffer, VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);

	// Draw the bound buffer (coords)
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}
