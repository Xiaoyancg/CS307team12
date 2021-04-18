#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
    // Calling with offset={2,2} would move the camera's position 2 pixels in the positive x and y direction
    void Camera::offsetPosition(glm::ivec2 offset) {
        // If there's nothing to offset, return
        if (offset.x == 0 && offset.y == 0) {
            return;
        }

        // Scale the position in pixels to the expected dimensions
        float dimx = mDimensions.x / 2;
        float dimy = mDimensions.y / 2;
        glm::vec4 ret = glm::ortho(dimx, -dimx, dimy, -dimy) * glm::vec4(offset.x, offset.y, 0.0f, 1.0f);

        // Rescale the offset to the Camera's projection (this just means the offset will considered with the zoom of the camera)
        ret = glm::inverse(getOrtho()) * ret;

        // Add window offset->camera projection offset
        mPosition.x += ret.x;
        mPosition.y += ret.y;
        
    }

    // Get matrices
    glm::mat4 Camera::getTranslate() {
        return glm::translate(glm::mat4(1), glm::vec3(mPosition, 0));
    }
    glm::mat4 Camera::getOrtho() {
        glm::vec2 dims = glm::vec2(mDimensions.x * mZoom, mDimensions.y * mZoom);
        float dimX = dims.x / 2;
        float dimY = dims.y / 2;
        glm::mat4 ortho = glm::ortho(-dimX, dimX, -dimY, dimY);
        return ortho;
    }

    glm::mat4 Camera::getMatrix() {
        return getOrtho() * getTranslate();
    }
}