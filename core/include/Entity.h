#pragma once
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

	// Calculate the coordinates of the corners based on location and scale
	// This will set 'mCoords'
	void calculateCoords(glm::vec2 location, glm::vec2 scale);

	// Set mLocation
	void setLocation(glm::vec2 location);

	// Get mLocation
	glm::vec2 getLocation();

	// Set mScale
	void setScale(glm::vec2 scale);

	// Get mScale
	glm::vec2 getScale();

	// Render the given entity on the current context
	// Assumes the shaders have already been setup
	void render();

	


private:
	// Basic Entity variables
	glm::vec2 mLocation; // Center of the entity (x, y)
	glm::vec2 mScale; // (x width, y height)
	double mRotation;
	float mCoords[8]; // Stores 4 pairs of (x, y) float coordinates (8 total), one for each corner of the entity
	
	// Sprite ID used to index sprites loaded into memory from the Game class
	// This isn't used yet though, I'm just adding it for later
	int mSpriteID;
};