#pragma once
#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "SpriteManager.h"

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
        Entity(std::string, glm::vec2 location, glm::vec2 scale, double rotation, int spriteID);

        Entity(std::string);
        // Calculate the coordinates of the corners based on location and scale
        // This will set 'mCoords'
        void calculateCoords(glm::vec2 location, glm::vec2 scale);

        // Render the given entity on the current context
        // Assumes the shaders have already been setup
        void render();

        static Entity *parse(nlohmann::json &root);
        nlohmann::json serialize();

        // =========================
        // CONSTRUCTOR

        // =========================
        // ATTRIBUTES OPERATION
        std::string getName();
        void setName(std::string);  // For variable name (setName(x);)
        void setName(const char *); // For constant "Name" (setName("EntityAHHHHH");)
        // =========================
        // PROPERTY OPERATION

        void setProperty(glm::vec2, glm::vec2, double, int);

        // Set mLocation
        void setLocation(glm::vec2 location);
        // Get mLocation
        glm::vec2 getLocation();

        // Set mScale
        void setScale(glm::vec2 scale);
        // Get mScale
        glm::vec2 getScale();

        void setRotation(double);
        double getRotation();

        void setSpriteID(int);
        int getSpriteID();

        bool isControlledEntity();
        void setControlledEntity(bool value);

        // =========================
        // MEMBER OPERATION

        // =========================
        // UTILITY OPERATION

        // AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
        // This is set by Game.cpp when a Game object is created
        // Any class that renders sprites needs a way to translate between Parchment Sprite IDs and imported OpenGL IDs.
        // The only other solution I could think of was to make mGameSprites in Game.cpp a global variable,
        // but that sounds like a disgusting solution and im not about to break encapsulation.
        // If anyone has a better idea, feel free to implement it. Otherwise, this should be fine.
        // Thank you Josh for the comments here! I'll move it to Core, along with logic
        static inline SpriteManager *mGameSprites = nullptr;

        int getEntityId();
        void setEntityId(int entityId);

    private:
        // Basic Entity variables
        std::string mEntityName;
        glm::vec2 mLocation; // Center of the entity (x, y)
        glm::vec2 mScale;    // (x width, y height)
        double mRotation = 0;
        int mCoords[16]; // Stores 4 pairs of (x, y) int coordinates (8 total), one for each corner of the entity

        // Sprite ID used to index sprites loaded into memory from the Game class
        // This isn't used yet though, I'm just adding it for later
        int mSpriteID = -1;


        int _entityId;

        bool mControlledEntity;

    };
}