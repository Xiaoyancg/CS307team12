#include "Entity.h"
#include <glad/glad.h>
#ifdef __TEST_CORE
#include <TestCore.h>
#endif // __TEST_CORE

namespace Core
{
// Constructor of the game class
    Entity::Entity ( std::string s, glm::vec2 location, glm::vec2 scale, double rotation, int spriteID )
        : mEntityName ( s ),
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
    std::string Entity::getName ()
    {
        return this->mEntityName;
    }
    
    // Sets name based on a given string variable
    void Entity::setName(std::string name) {
        this->mEntityName = name;
    }

    // Sets name based on a constant string, for example setName("Entity1")
    void Entity::setName(const char* name) {
        this->mEntityName = std::string(name);
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




    Entity::Entity ( std::string s )
        : mEntityName ( s ) {}

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
        // P1 texture coords
        mCoords[2] = 0;
        mCoords[3] = 1;

        // P2
        mCoords[4] = lowX; // Bottom left x
        mCoords[5] = lowY; // Bottom left y
        // P2 texture coords
        mCoords[6] = 0;
        mCoords[7] = 0;

        // P3
        mCoords[8] = highX; // Top right x
        mCoords[9] = highY; // Top right y
        // P3 texture coords
        mCoords[10] = 1;
        mCoords[11] = 1;

        // P4
        mCoords[12] = highX; // Bottom right x
        mCoords[13] = lowY; // Bottom right y
        // P4 texture coords
        mCoords[14] = 1;
        mCoords[15] = 0;
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

    void Entity::render ()
    {
        if (mSpriteID != -1 && mGameSprites->atID(mSpriteID)) {
            glBindTexture(GL_TEXTURE_2D, mGameSprites->atID(mSpriteID)->getOpenGLTextureID()); // Bind correct sprite
        }
        // Load the data of the 'coords' buffer into the currently bound array buffer, VBO
        glBufferData ( GL_ARRAY_BUFFER, sizeof ( mCoords ), mCoords, GL_DYNAMIC_DRAW );
        // Draw the bound buffer (coords)
        glDrawArrays ( GL_TRIANGLE_STRIP, 0, 4 );

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind current sprite

#ifdef __TEST_CORE
        entityError = glGetError ();
#endif // __TEST_CORE
    }


    using json = nlohmann::json;

    Entity* Entity::parse(json& root) {
        Entity* entity = new Entity(root.at("EntityName").get<std::string>());

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
        entity->setSpriteID(root.at("spriteID").get<int>());

        return entity;
    }

}
