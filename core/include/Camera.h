#pragma once
#include <glm/glm.hpp>

namespace Core 
{

    /*
     * 2D camera with only a position attribute. 
     */
    class Camera {
    public:
        Camera();

        // Set position of camera
        glm::ivec3 getPosition();
        void setPosition(glm::ivec3 position);
        void offsetPosition(glm::ivec3 offset);

        // Returns the matrix transformation used in the main shader
        glm::mat4 getMatrix();

    private:
        glm::ivec3 mPosition;
    };

}