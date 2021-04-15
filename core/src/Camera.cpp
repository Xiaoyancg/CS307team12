#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Core {
    Camera::Camera() {
        // Set position of the camera, defaults to (0, 0)
        mPosition = glm::ivec2(0, 0);

        // Set dimensions of camera, defaults to (1280, 720) which is the size of the Game viewport
        // This can be changed, but I think it might be assumed to be these dimensions somewhere else in the code
        mDimensions = glm::ivec2(1280, 720);

        // Set zoom of the camera, defaults to 1.0f
        // zoom < 1 will shrink the image, zoom > 1 will enlarge image
        mZoom = 1.0f;
    }

    // Position
    glm::ivec2 Camera::getPosition() {
        return mPosition;
    }

    void Camera::setPosition(glm::ivec2 position) {
        mPosition = position;
    }

    void Camera::offsetPosition(glm::ivec2 offset) {
        mPosition.x += offset.x;
        mPosition.y += offset.y;
        // Using matrix transformation to link offsets to Camera projection has been a pain to implement...
        // It's working other than resetting each offset, meaning each right-click starts from (0,0), not from the last offset
        /*
        if (offset.x == 0 && offset.y == 0) {
            return;
        }
        float dimx = mDimensions.x / 2;
        float dimy = mDimensions.y / 2;
        glm::vec4 ret = glm::ortho(-dimx, dimx, -dimy, dimy) * glm::vec4(offset.x, offset.y, 0.0f, 1.0f);
        ret = glm::inverse(getOrtho()) * ret;
        //ret = glm::inverse(getTranslate()) * ret;
        //ret = glm::inverse(glm::translate(glm::mat4(1), glm::vec3(ret.x, ret.y, 0))) * ret;

        printf("offsetting (%d %d)->(%f %f)\n", offset.x, offset.y, ret.x , ret.y);
        printf("mposition bef %d %d\n", mPosition.x, mPosition.y);
        mPosition.x += ret.x;
        mPosition.y += ret.y;
        printf("mposition aft %d %d\n", mPosition.x, mPosition.y);
        */
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