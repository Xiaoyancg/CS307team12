#include "Entity.h"
#include <glad/glad.h>
#ifdef __TEST_CORE
#include <test_core.h>
#endif // __TEST_CORE


extern int width; // Width of the window, set in Game.cpp
extern int height; // Height of the window, set in Game.cpp

// Constructor of the game class
Entity::Entity(glm::vec2 location, glm::vec2 scale, double rotation, int spriteID)
: mLocation(location),
mScale(scale),
mRotation(rotation), 
mSpriteID(spriteID) {
	// All this constructor does is calculate the coordinates of the 4 corners of the entity, based on location and scale
	calculateCoords(location, scale);
}

void Entity::calculateCoords(glm::vec2 location, glm::vec2 scale) {
	// Get the distances to the left/right and top/bottom of the entity from the center
	float halfScaleWidth = scale.x / 2;
	float halfScaleHeight = scale.y / 2;

	// Calculate the smallest and greatest x and y (combinations of these make the 4 corners of the entity)
	float lowX = location.x - halfScaleWidth;
	float highX = location.x + halfScaleWidth;
	float lowY = location.y - halfScaleHeight;
	float highY = location.y + halfScaleHeight;

	// P1
	mCoords[0] = lowX; // Top left x
	mCoords[1] = highY; // Top left y
	// P2
	mCoords[2] = lowX; // Bottom left x
	mCoords[3] = lowY; // Bottom left y
	// P3
	mCoords[4] = highX; // Top right x
	mCoords[5] = highY; // Top right y
	// P4
	mCoords[6] = highX; // Bottom right x
	mCoords[7] = lowY; // Bottom right y

}

void Entity::setLocation(glm::vec2 location) {
	mLocation = location;
	calculateCoords(mLocation, mScale);
}

glm::vec2 Entity::getLocation() {
	return mLocation;
}

void Entity::setScale(glm::vec2 scale) {
	mScale = scale;
	calculateCoords(mLocation, mScale);
}

glm::vec2 Entity::getScale() {
	return mScale;
}

void Entity::render() {
	// Load the data of the 'coords' buffer into the currently bound array buffer, VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(mCoords), mCoords, GL_DYNAMIC_DRAW);

	// Draw the bound buffer (coords)
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#ifdef __TEST_CORE
	entityError = glGetError ();
#endif // __TEST_CORE


}
