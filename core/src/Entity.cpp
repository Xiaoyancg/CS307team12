#include "Entity.h"
#include <glad/glad.h>
#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
// Constructor of the game class
    Entity::Entity ( std::string s, glm::vec2 location, glm::vec2 scale, double rotation, int spriteID )
        : entityName ( s ),
        mLocation ( location ),
        mScale ( scale ),
        mRotation ( rotation ),
        mSpriteID ( spriteID )
    {
// All this constructor does is calculate the coordinates of the 4 corners of the entity, based on location and scale
        calculateCoords ( location, scale );
    }


        // =========================
        // CONSTRUCTOR

        // =========================
        // ATTRIBUTES OPERATION
    std::string &Entity::getName ()
    {
        return this->entityName;
    }
        // =========================
        // PROPERTY OPERATION
    void Entity::setProperty ( glm::vec2 l, glm::vec2 s, double r, int sid)
    {
        this->mLocation = l;
        this->mScale = s;
        this->mRotation = r;
        this->mSpriteID = sid;
        calculateCoords ( mLocation, mScale );
    }
        // =========================
        // MEMBER OPERATION

        // =========================
        // UTILITY OPERATION

    double Entity::getRotation ()
    {
        return this->mRotation;
    }
    void Entity::setRotation ( double d )
    {
        this->mRotation = d;
    }
    void Entity::setSpriteID ( int i )
    {
        this->mSpriteID = i;
    }
    int Entity::getSpriteID ()
    {
        return this->mSpriteID;
    }




    Entity::Entity ( std::string s ):entityName ( s )
    {

    }

    void Entity::calculateCoords ( glm::vec2 location, glm::vec2 scale )
    {
// Get the distances to the left/right and top/bottom of the entity from the center
        int halfScaleWidth = scale.x / 2;
        int halfScaleHeight = scale.y / 2;

        // Calculate the smallest and greatest x and y (combinations of these make the 4 corners of the entity)
        int lowX = location.x - halfScaleWidth;
        int highX = location.x + halfScaleWidth;
        int lowY = location.y - halfScaleHeight;
        int highY = location.y + halfScaleHeight;

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

    void Entity::setLocation ( glm::vec2 location )
    {
        mLocation = location;
        calculateCoords ( mLocation, mScale );
    }

    glm::vec2 Entity::getLocation ()
    {
        return mLocation;
    }

    void Entity::setScale ( glm::vec2 scale )
    {
        mScale = scale;
        calculateCoords ( mLocation, mScale );
    }

    glm::vec2 Entity::getScale ()
    {
        return mScale;
    }

    void Entity::setRotation(double rotation) {
        mRotation = rotation;
    }

    double Entity::getRotation() {
        return mRotation;
    }

    void Entity::setSpriteID(int spriteID) {
        mSpriteID = spriteID;
    }

    int Entity::getSpriteID() {
        return mSpriteID;
    }

    void Entity::render ()
    {
// Load the data of the 'coords' buffer into the currently bound array buffer, VBO
        glBufferData ( GL_ARRAY_BUFFER, sizeof ( mCoords ), mCoords, GL_DYNAMIC_DRAW );
        // Draw the bound buffer (coords)
        glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );
#ifdef __TEST_CORE
        entityError = glGetError ();
#endif // __TEST_CORE


    }


    using json = nlohmann::json;

    Entity* Entity::parse(json& root) {
        Entity* entity = new Entity(root.at("Name").get<std::string>());

        std::vector<json> locVec = root.at("location").get<std::vector<json>>();
        entity->setLocation(glm::vec2(
            locVec[0].get<float>(),
            locVec[1].get<float>()
        ));
        std::vector<json> scaleVec = root.at("scale").get<std::vector<json>>();
        entity->setScale(glm::vec2(
            scaleVec[0].get<float>(),
            scaleVec[1].get<float>()
        ));

        entity->setRotation(root.at("rotation").get<double>());
        entity->setSpriteID(root.at("Entity ID").get<int>());

        return entity;
    }

}
