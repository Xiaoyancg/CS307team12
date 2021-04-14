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

        // Position
        glm::ivec2 getPosition();
        void setPosition(glm::ivec2 position);
        void offsetPosition(glm::ivec2 offset);

        // Dimensions
        void setDimensions(int x, int y);
        glm::ivec2 getDimensions();

        // Zoom
        void setZoom(float zoom);
        float getZoom();


        glm::mat4 getTranslate();
        glm::mat4 Camera::getOrtho();


        // Returns the matrix transformation used in the main shader
        glm::mat4 getMatrix();

    private:
        glm::ivec2 mPosition;
        glm::ivec2 mDimensions;
        float mZoom; // Defaults to 1, meaning no zoom. <1 means zoom out, >1 means zoom in
    };

}