#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Core
{


    Camera::Camera(glm::vec2 dimensions, glm::vec2 position, float zoom)
        : mPosition(position), mDimensions(dimensions), mZoom(zoom), mLockedEntityID(CAMERA_NO_LOCKED_ENTITY), mLockBounds(0, 0), mCurrPage(nullptr)
    {
    }

    // Position
    glm::vec2 Camera::getPosition()
    {
        return mPosition;
    }

    void Camera::setPosition(glm::vec2 position)
    {
        mPosition = position;
    }

    // Offsets the position of the camera based on a click-drag offset in pixels. The pixels are scaled based on the cameras variables.
    // This means for exact movemements where the zoom/scale should not be applied (like for distances between entities or something), 
    // use the function offsetPositionExact() (this is private rn, but make it public if you need it outside of this class).
    // offsetPosition should mainly be used with mouse coordinates on top of an ImGui window.
    void Camera::offsetPosition(glm::vec2 offset) {
        // If there's nothing to offset, return
        if (offset.x == 0 && offset.y == 0)
        {
            return;
        }

        // Scale the position in pixels to the expected dimensions
        float dimx = mDimensions.x / 2;
        float dimy = mDimensions.y / 2;
        glm::vec4 ret = glm::ortho(-dimx, dimx, -dimy, dimy) * glm::vec4(offset.x, offset.y, 0.0f, 1.0f);

        // Rescale the offset to the Camera's projection (this just means the offset will considered with the zoom of the camera)
        ret = glm::inverse(getOrtho()) * ret;

        // Add window offset->camera projection offset
        mPosition.x += ret.x;
        mPosition.y += ret.y;
    }


    // Offsets the position of the camera based on a click-drag offset in pixels. The pixels are scaled based on the cameras variables
    void Camera::offsetPositionExact(int x, int y) {
        // If there's nothing to offset, return
        if (x == 0 && y == 0) {
            return;
        }

        // Add window offset->camera projection offset
        mPosition.x += x;
        mPosition.y += y;
    }


    // Given coordinates that fulfill `(0,0) < (x,y) < (1,1)`,
    // project to world coordinates at that position
    glm::vec2 Camera::projectToWorld(glm::vec2 coords) {
        glm::vec4 normalized = glm::vec4(coords.x * 2 - 1, coords.y * 2 - 1, 0, 1);
        glm::vec4 transformed = glm::inverse(getMatrix()) * normalized;
        return glm::vec2(transformed.x, transformed.y);
    }

    // Get matrices
    glm::mat4 Camera::getTranslate() {
        // Update Camera position based on lockedEntity location if the camera is locked
        if (mLockedEntityID >= 0 && mCurrPage) {
            Entity* mLockedEntity = mCurrPage->getEntityWithID(mLockedEntityID);
            if (mLockedEntity) {
                glm::vec2 entLoc = mLockedEntity->getLocation();
                glm::vec2 diff(entLoc.x - (float)mPosition.x, entLoc.y - (float)mPosition.y);

                if (diff.x > mLockBounds.x) {
                    // Move camera right
                    offsetPositionExact(diff.x - mLockBounds.x, 0);
                }             
                else if (diff.x < -mLockBounds.x) {
                    // Move camera left
                    offsetPositionExact(diff.x + mLockBounds.x, 0);
                }
                if (diff.y > mLockBounds.y) {
                    // Move camera up
                    offsetPositionExact(0, diff.y - mLockBounds.y);
                }             
                else if (diff.y < -mLockBounds.y) {
                    // Move camera down
                    offsetPositionExact(0, diff.y + mLockBounds.y);
                }
            }
        }


        return glm::translate(glm::mat4(1), glm::vec3(-mPosition, 0));
    }
    glm::mat4 Camera::getOrtho()
    {
        glm::vec2 dims = glm::vec2(mDimensions.x / mZoom, mDimensions.y / mZoom);
        float dimX = dims.x / 2.0f;
        float dimY = dims.y / 2.0f;
        glm::mat4 ortho = glm::ortho(-dimX, dimX, -dimY, dimY);
        return ortho;
    }

    glm::mat4 Camera::getMatrix()
    {
        return getOrtho() * getTranslate();
    }

    void Camera::use() {
        GLint program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
        cameraUniform = glGetUniformLocation(program, "camera");
        glUniformMatrix4fv(cameraUniform, 1, GL_FALSE, glm::value_ptr(getMatrix()));
    }
}