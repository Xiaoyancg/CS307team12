#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
    Camera::Camera() {
        // Set position of the camera, defaults to (0, 0)
        mPosition = glm::ivec3(0, 0, 0);

        // Set dimensions of camera, defaults to (1280, 720) which is the size of the Game viewport
        // This can be changed, but I think it might be assumed to be these dimensions somewhere else in the code
        mDimensions = glm::ivec2(1280, 720);

        // Set zoom of the camera, defaults to 1.0f
        // zoom < 1 will shrink the image, zoom > 1 will enlarge image
        mZoom = 1.0f;
    }

    // Position
    glm::ivec3 Camera::getPosition() {
        return mPosition;
    }

    void Camera::setPosition(glm::ivec3 position) {
        mPosition = position;
    }

    void Camera::offsetPosition(glm::ivec3 offset) {
        mPosition += offset;
    }

    // Dimensions
    void Camera::setDimensions(int x, int y) {
        mDimensions = glm::ivec2(x, y);
    }
    glm::ivec2 Camera::getDimensions() {
        return mDimensions;
    }

    // Zoom
    void Camera::setZoom(float zoom) {
        mZoom = zoom;
    }
    float Camera::getZoom() {
        return mZoom;
    }


    glm::mat4 Camera::getMatrix() {
        glm::vec2 dims = glm::vec2(mDimensions.x * mZoom, mDimensions.y * mZoom);
        float dimX = dims.x / 2;
        float dimY = dims.y / 2;
        glm::mat4 ortho = glm::ortho(-dimX, dimX, -dimY, dimY);
        glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(mPosition));
        return ortho * translate;
    }
}