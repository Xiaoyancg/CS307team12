#include "windows/GameWindow.h"


void GameWindow::draw()
{
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        if (editor->getGamePtr() != nullptr)
        {
            // set the windows default size (not needed for game window)
            //ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            // the game view window itself
            ImGui::Begin("Game View", &visible);
            updateWindowFocus();

            if (!editor->isGameRunning()) {
                if (ImGui::Button("Run")) {
                    editor->runGame();
                }
            } else {
                if (ImGui::Button("Stop")) {
                    editor->stopGame();
                }
            }

            // Get size of drawable space on the window, instead of the entire size of the window
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();

            glBindTexture(GL_TEXTURE_2D, mTexCBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, editor->getGamePtr()->width, editor->getGamePtr()->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            glViewport(0, 0, editor->getGamePtr()->width, editor->getGamePtr()->height); // Set viewport to the Game dimensions
            glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
            editor->getGamePtr()->render(); // Render Game with new viewport size
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ImGui::Image((ImTextureID) mTexCBO, ImVec2(editor->getGamePtr()->width, editor->getGamePtr()->height), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}
