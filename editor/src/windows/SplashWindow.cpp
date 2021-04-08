#include "windows/SplashWindow.h"

#include <stb_image_aug.h>
#include <iostream>


void SplashWindow::draw() {
    if (visible)
    {
        ImGui::SetNextWindowSize(ImVec2(500 * 1.5, 400 * 1.5), ImGuiCond_Appearing);
        //ImGui::SetNextWindowContentSize(ImVec2(500*1.5, 360*1.5));
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        //ImGui::OpenPopup("Splash");
        if (ImGui::Begin("Splash", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
        {
            // Load from file
            int image_width = 0;
            int image_height = 0;
            unsigned char *image_data = stbi_load("./parchment_splash.png", &image_width, &image_height, NULL, 4);

            if (image_data == NULL)
            {
#ifdef __TEST_EDITOR
                splashImageFail = true;
#endif
                setVisible(false);
                return;
            }

            else
            {
                // Create a OpenGL texture identifier
                GLuint image_texture;
                glGenTextures(1, &image_texture);
                glBindTexture(GL_TEXTURE_2D, image_texture);

                // Setup filtering parameters for display
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

                // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
                stbi_image_free(image_data);

                //ImVec2 canvas_size = ImGui::GetContentRegionAvail();
                ImVec2 canvas_size = ImVec2(500 * 1.5, 363 * 1.5);
                ImGui::Image((void *)(intptr_t)image_texture, canvas_size);

                // Display Parchment version and current date-time
                ImGui::Text("Parchment Editor v0.20");
                ImGui::SameLine(ImGui::GetWindowWidth() - 180);
                auto time = std::chrono::system_clock::now();
                std::time_t current_time = std::chrono::system_clock::to_time_t(time);
                ImGui::Text(std::ctime(&current_time));
            }
            // CLOSE SPLASH ON CLICK OUTSIDE LIKE A POPUP BUT NOT BECAUSE FUCK ME I GUESS
            if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            {
                setVisible(false);
            }

            ImGui::End();
#ifdef __TEST_EDITOR

#endif
        }
        ImGui::PopStyleVar();
    }
}
