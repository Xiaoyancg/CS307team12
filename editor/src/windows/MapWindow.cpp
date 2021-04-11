#include "windows/MapWindow.h"


void MapWindow::draw()
{
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        if (editor->getCurrentMap() != nullptr)
        {
            Core::Map *currMap = editor->getCurrentMap();
            // set the windows default size
            ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            // the game view window itself
            ImGui::Begin("Map View", &visible);
            updateWindowFocus();

            // Get size of drawable space on the window, instead of the entire size of the window
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();
            glm::ivec2 dims = currMap->getDimensions();
            int tileSize = currMap->getTileSize();
            int mapWidth = (dims.x+1) * tileSize;
            int mapHeight = (dims.y+1) * tileSize;
            //ImGui::SetWindowSize(ImVec2(mapWidth, mapHeight));

            Core::MapPage* mapPage = editor->getGamePtr()->getDefaultMapPage();
            glm::vec4& bgCol = mapPage->GetBackgroundColor();

            glBindTexture(GL_TEXTURE_2D, mMapTexCBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapWidth, mapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float bgColArr[] = { bgCol.r, bgCol.g, bgCol.b, bgCol.a };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bgColArr);
            glBindTexture(GL_TEXTURE_2D, 0);

            glViewport(
                0, 0,
                (int) mapWidth, (int) mapHeight
            );
            Core::Game* game = editor->getGamePtr();
            glm::vec2 prevScale = game->getShaderScale();
            game->setShaderScale(glm::vec2(mapWidth, mapHeight));
            //glViewport(0, 0, 1000, 1000);
            glBindFramebuffer(GL_FRAMEBUFFER, mMapFBO);

            glClearColor(bgCol.r, bgCol.g, bgCol.b, bgCol.a);
            glClear(GL_COLOR_BUFFER_BIT);
            editor->getGamePtr()->renderDefaultMapPage(); // Render Game with new viewport size
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
            game->setShaderScale(prevScale);

            ImVec2 uv0 = ImVec2(0.5f - (0.5f*canvas_size.x)/mapWidth, 0.5f - (0.5f*canvas_size.y)/mapHeight);
            ImVec2 uv1 = ImVec2(0.5f + (0.5f*canvas_size.x)/mapWidth, 0.5f + (0.5f*canvas_size.y)/mapHeight);
            ImGui::Image((ImTextureID) mMapTexCBO, ImVec2(canvas_size.x, canvas_size.y), uv0, uv1);

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}