#pragma once
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace Core {
    bool print = false;

    Camera::Camera(void) {
        mPosition = glm::ivec3(0, 0, 0);
    }


    glm::ivec3 Camera::getPosition() {
        return mPosition;
    }

    void Camera::setPosition(glm::ivec3 position) {
        mPosition = position;
    }

    void Camera::offsetPosition(glm::ivec3 offset) {
        mPosition += offset;
        print = true;
    }


    glm::mat4 Camera::getMatrix() {
        //printf("%d %d %d\n", mPosition.x, mPosition.y, mPosition.z);
        glm::mat4 ortho = glm::ortho(0.0f, 2.0f, 0.0f, 2.0f);
        //glm::mat4 ortho = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f);

        glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(mPosition));
        //glm::mat4 ret = ortho * translate;
        glm::mat4 ret = translate;
        if (print) {
            std::cout << glm::to_string(ret) << std::endl;
            print = false;
        }
        return ret;
    }
}