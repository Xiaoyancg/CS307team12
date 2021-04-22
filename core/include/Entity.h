#pragma once
#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "SpriteManager.h"

// constant for the default ID for entities. If this gets changed to -2, Camera code will break. This is used instead of -1 for consistency
#define NO_ENTITY_ID -1

namespace Core
{
    class Page;
    // Entity class
    class Entity
    {
    public:
        // Constructor, just sets the coordinates of the entity
        // 'location' is the (x, y) coordinate of the Entity
        // 'scale' is the (width, height) of the Entity in pixels
        // 'rotation' is the rotation of the entity
        // spriteID is unimplemented, but will reference a specific loaded sprite
        Entity(std::string, glm::vec2 location, glm::vec2 scale, double rotation, int spriteID, bool invisible);
        Entity(std::string);
        explicit Entity(const Entity &other);

        // Calculate the coordinates of the corners based on location and scale
        // This will set 'mCoords'
        void calculateCoords(glm::vec2 location, glm::vec2 scale);

        // Render the given entity on the current context
        // Assumes the shaders have already been setup
        void render();

        static Entity *fromJSON(nlohmann::json &root);
        void parse(nlohmann::json &root);
        nlohmann::json serialize();

        // =========================
        // CONSTRUCTOR

        // =========================
        // ATTRIBUTES OPERATION
        std::string getName();
        void setName(std::string); // For variable name (setName(x);)
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

        bool isInvisible()
        {
            return mIsInvisible;
        }
        void setInvisible(bool value)
        {
            mIsInvisible = value;
        }

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

        Page *getParentPage()
        {
            return mParentPage;
        }

        void setParentPage(Page *page)
        {
            mParentPage = page;
        }
        void setInScriptId(int);
        int getInScriptId();
        void setOutScriptId(int);
        int getOutScriptId();

    private:
        // Basic Entity variables
        std::string mEntityName;
        glm::vec2 mLocation; // Center of the entity (x, y)
        glm::vec2 mScale;    // (x width, y height)
        double mRotation = 0;
        float mCoords[16]; // Stores 4 pairs of (x, y) int coordinates (8 total), one for each corner of the entity

        // Sprite ID used to index sprites loaded into memory from the Game class
        // This isn't used yet though, I'm just adding it for later
        int mSpriteID = -1;

        int _entityId = NO_ENTITY_ID;

        bool mIsInvisible = false;

        Page *mParentPage = nullptr;

        /// \brief when the entity enter, game should run this (custom) script
        ///
        int mInScriptId = -1;
        int mOutScriptId = -1;
    };
}