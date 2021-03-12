#pragma once
#include <glm/glm.hpp>
#include <string>

namespace Core
{
// Entity class
	class Entity
	{
	public:
		// Constructor, just sets the coordinates of the entity
		// 'location' is the (x, y) coordinate of the Entity
		// 'scale' is the (width, height) of the Entity in pixels
		// 'rotation' is the rotation of the entity
		// spriteID is unimplemented, but will reference a specific loaded sprite
		Entity ( std::string, glm::vec2 location, glm::vec2 scale, double rotation, int spriteID );

		Entity ( std::string );
		// Calculate the coordinates of the corners based on location and scale
		// This will set 'mCoords'
		void calculateCoords ( glm::vec2 location, glm::vec2 scale );

		// Set mLocation
		void setLocation ( glm::vec2 location );

		// Get mLocation
		glm::vec2 getLocation ();

		// Set mScale
		void setScale ( glm::vec2 scale );

		// Get mScale
		glm::vec2 getScale ();

		// Render the given entity on the current context
		// Assumes the shaders have already been setup
		void render ();


		// =========================
		// CONSTRUCTOR

		// =========================
		// ATTRIBUTES OPERATION
		std::string &getName ();
		// =========================
		// PROPERTY OPERATION
		void setProperty ( glm::vec2, glm::vec2, double, int );
		// =========================
		// MEMBER OPERATION

		// =========================
		// UTILITY OPERATION




	private:
		// Basic Entity variables
		std::string entityName;
		glm::vec2 mLocation; // Center of the entity (x, y)
		glm::vec2 mScale; // (x width, y height)
		double mRotation;
		int mCoords[8]; // Stores 4 pairs of (x, y) int coordinates (8 total), one for each corner of the entity

		// Sprite ID used to index sprites loaded into memory from the Game class
		// This isn't used yet though, I'm just adding it for later
		int mSpriteID;
	};
}