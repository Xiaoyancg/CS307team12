#pragma once
#include <glm/glm.hpp>

namespace Core 
{

    /*
     * 2D camera with only a position attribute. 
     */
    class Camera {
    public:
        Camera(
            glm::ivec2 dimensions = glm::ivec2(1280, 720),
            glm::ivec2 position = glm::ivec2(0, 0),
            float zoom = 1.0f
        ) : mPosition(position), mDimensions(dimensions), mZoom(zoom) {}

        // Position
        glm::ivec2 getPosition() {
            return mPosition;
        }
        void setPosition(glm::ivec2 position) {
            mPosition = position;
        }
        void offsetPosition(glm::ivec2 offset);

        // Dimensions
        void setDimensions(int x, int y) {
            setDimensions(glm::ivec2(x, y));
        }
        void setDimensions(glm::ivec2 dimensions) {
            mDimensions = dimensions;
        }
        glm::ivec2 getDimensions() {
            return mDimensions;
        }

        // Zoom
        void setZoom(float zoom) {
            if (zoom > 0) {
                mZoom = zoom;
            }
        }
        float getZoom() {
            return mZoom;
        }

        glm::mat4 getTranslate();
        glm::mat4 Camera::getOrtho();
        // Returns the matrix transformation used in the main shader
        glm::mat4 getMatrix(); // getOrtho * getTranslate

    private:
        glm::ivec2 mPosition;
        glm::ivec2 mDimensions;
        float mZoom; // Defaults to 1, meaning no zoom. <1 means zoom out, >1 means zoom in
    };

}