#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
    Camera::Camera()
    {
        // Set position of the camera, defaults to (0, 0)
        mPosition = glm::ivec2(0, 0);

        // Set dimensions of camera, defaults to (1280, 720) which is the size of the Game viewport
        // This can be changed, but I think it might be assumed to be these dimensions somewhere else in the code
        mDimensions = glm::ivec2(1280, 720);

        // Set zoom of the camera, defaults to 1.0f
        // zoom < 1 will shrink the image, zoom > 1 will enlarge image
        mZoom = 1.0f;
    }
    Camera::Camera(
        glm::ivec2 dimensions = glm::ivec2(1280, 720),
        glm::ivec2 position = glm::ivec2(0, 0),
        float zoom = 1.0f) : mPosition(position), mDimensions(dimensions), mZoom(zoom)
    {
    }

    // Position
    glm::ivec2 Camera::getPosition()
    {
        return mPosition;
    }

    void Camera::setPosition(glm::ivec2 position)
    {
        mPosition = position;
    }

    // Offsets the position of the camera based on a click-drag offset in pixels. The pixels are scaled based on the cameras variables
    void Camera::offsetPosition(glm::ivec2 offset)
    {
        // If there's nothing to offset, return
        if (offset.x == 0 && offset.y == 0)
        {
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
    glm::mat4 Camera::getTranslate()
    {
        return glm::translate(glm::mat4(1), glm::vec3(mPosition, 0));
    }
    glm::mat4 Camera::getOrtho()
    {
        glm::vec2 dims = glm::vec2(mDimensions.x * mZoom, mDimensions.y * mZoom);
        float dimX = dims.x / 2;
        float dimY = dims.y / 2;
        glm::mat4 ortho = glm::ortho(-dimX, dimX, -dimY, dimY);
        return ortho;
    }

    glm::mat4 Camera::getMatrix()
    {
        return getOrtho() * getTranslate();
    }
}