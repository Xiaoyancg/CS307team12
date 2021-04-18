#pragma once

#include "windows/Window.h"
#include <stb_image_aug.h>


class SplashWindow : public Window {
public:
    SplashWindow(Editor* editor, ImVec2 size);
    ~SplashWindow() {
        stbi_image_free(image_data);
    }
    void draw();
private:
    GLuint imageTexture = -1;
    int image_width = -1;
    int image_height = -1;
    unsigned char* image_data = nullptr;
};
