#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Core
{

    /*
     * 2D camera with only a position attribute. 
     */
    class Camera
    {
    public:
        Camera(
            glm::vec2 dimensions = glm::vec2(1280, 720),
            glm::vec2 position = glm::vec2(0, 0),
            float zoom = 1.0
        );

        // Position
        glm::vec2 getPosition();
        void setPosition(float x, float y) {
            setPosition(glm::vec2(x, y));
        }
        void setPosition(glm::vec2 position);
        void offsetPosition(glm::vec2 offset);

        // Dimensions
        void setDimensions(float x, float y)
        {
            setDimensions(glm::vec2(x, y));
        }
        void setDimensions(glm::vec2 dimensions)
        {
            mDimensions = dimensions;
        }
        glm::vec2 getDimensions()
        {
            return mDimensions;
        }

        // Zoom
        void setZoom(float zoom)
        {
            if (zoom > 0)
            {
                mZoom = zoom;
            }
        }
        float getZoom()
        {
            return mZoom;
        }

        glm::vec2 projectToWorld(glm::vec2 coords);
        glm::mat4 getTranslate();
        glm::mat4 getOrtho();
        // Returns the matrix transformation used in the main shader
        glm::mat4 getMatrix(); // getOrtho * getTranslate

        void use();

    private:
        glm::vec2 mPosition;
        glm::vec2 mDimensions;
        float mZoom; // Defaults to 1, meaning no zoom. <1 means zoom out, >1 means zoom in
        GLuint cameraUniform;
    };

}