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
            //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

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
                // auto now = std::chrono::steady_clock::now();
                // float dt = (float) (now - lastTime).count() / std::chrono::steady_clock::period::den;
                // lastTime = now;
                editor->getGamePtr()->logicLoop();
            }
            
            ImGui::Separator();

            glViewport(0, 0, editor->getGamePtr()->width, editor->getGamePtr()->height); // Set viewport to the Game dimensions
            glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
            editor->getGamePtr()->render(); // Render Game with new viewport size
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ImVec2 windowSize = ImGui::GetWindowSize();
            float aspectRatio = (float) editor->getGamePtr()->width / editor->getGamePtr()->height;
            ImVec2 cursor = ImGui::GetCursorPos();
            cursor.y += 10;
            float gameHeight = windowSize.y - cursor.y - 10;
            float gameWidth = gameHeight * aspectRatio;
            if (gameWidth + 20 > windowSize.x) {
                gameWidth = windowSize.x - 20;
                gameHeight = gameWidth / aspectRatio;
            }

            cursor.x = (windowSize.x - gameWidth) / 2;
            ImGui::SetCursorPos(cursor);
            ImGui::Image((ImTextureID) mTexCBO, ImVec2(gameWidth, gameHeight), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
        }
    }
}
