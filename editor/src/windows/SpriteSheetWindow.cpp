#include "windows/SpriteSheetWindow.h"

void SpriteSheetWindow::draw() {
    if (visible)
    {
        if (editor->getCurrentComponentList()[CUR_SPRITESHEET] != "")
        {
            // set the windows default size (not needed for game window)
            //ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            // the game view window itself
            ImGui::Begin("SpriteSheet View", &visible);
            updateWindowFocus();

            // Get size of drawable space on the window, instead of the entire size of the window
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            Core::SpriteSheet* spritesheet = editor->getCurrentSpriteSheet();

            glBindTexture(GL_TEXTURE_2D, mSpriteSheetTexCBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spritesheet->getDimensions().x, spritesheet->getDimensions().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
            glViewport(0, 0, spritesheet->getDimensions().x, spritesheet->getDimensions().y); // Set viewport to the Game dimensions
            glBindFramebuffer(GL_FRAMEBUFFER, mSpriteSheetFBO);
            editor->getGamePtr()->renderSpriteSheet(spritesheet);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
            ImGui::Image((ImTextureID)mSpriteSheetTexCBO, ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}
