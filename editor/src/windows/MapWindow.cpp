#include "windows/MapWindow.h"

// handleClick will return a pointer to a Tile on the current Map based on whether
// a click has occured, or nullptr 
Core::Tile* MapWindow::handleClick() {
    // Calculate the min/max coords of the MapView window
    ImVec2 min = ImGui::GetWindowPos();
    ImVec2 max = min;
    min.y += ImGui::GetWindowContentRegionMin().y; // Get y value of window past the title bar
    max.x += ImGui::GetWindowContentRegionMax().x; // Get max x value of MapView
    max.y += ImGui::GetWindowContentRegionMax().y; // Get max y value of MapView

    ImVec2 bottomLeftCanvas;
    bottomLeftCanvas.x = min.x;
    bottomLeftCanvas.y = ImGui::GetWindowContentRegionMax().y;
    ImVec2 canvasDimensions(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x,
                            ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
    //printf("loc (%f, %f) dims (%f, %f)\n", bottomLeftCanvas.x, bottomLeftCanvas.y, canvasDimensions.x, canvasDimensions.y);

    //////////// Mouse handling within the MapView window ////////////
    // If mouse left is currently pressed and the MapView is focused
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowFocused()) {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y)) {
            // The click is valid within the MapView window.
            // Stretching introduces some problems with the coordinates we need to send to the core
            // So we set them on a scale of 0->1. They later get rescaled to the dimensions of the MapView in the core
            // This lets the core understand the coordinates regardless of window stretching.
            // NOTE: Right now the code just gets the clicked Tile and sets its sprite ID to 1


            glm::ivec2 center(canvasDimensions.x / 2, canvasDimensions.y / 2);
            float x = (click_pos.x - min.x);
            float y = canvasDimensions.y - (click_pos.y - min.y);
            glm::ivec2 diff(x - center.x, y - center.y);
            printf("DIFF %d %d\n", diff.x, diff.y);
            
            /*
            float x = (click_pos.x - min.x) / canvasDimensions.x;
            float y = 1 - ((click_pos.y - min.y) / canvasDimensions.y);
            x = 2 * (x - 0.5);
            y = 2 * (y - 0.5);
            */
            /*
            Core::Map* currMap = editor->getCurrentMap();
            glm::ivec2 dims = currMap->getDimensions();
            int tileSize = currMap->getTileSize();
            int mapWidth = (dims.x + 1) * tileSize;
            int mapHeight = (dims.y + 1) * tileSize;
            x *= (mapWidth/2);
            y *= (mapHeight/2);
            */
            //printf("click send > %f %f\n", x, y);
            //Core::Tile* clicked_tile = editor->getGamePtr()->getDefaultMapPage()->getTileFromClick(2 * (x - 0.5), 2 * (y - 0.5));
            Core::Tile* clicked_tile = editor->getGamePtr()->getDefaultMapPage()->getTileFromClick(diff.x, diff.y);

            if (clicked_tile) {
                clicked_tile->setSpriteID(1);
            }
        }
    }

    return nullptr;
}

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
            glViewport(0, 0, (int) mapWidth, (int) mapHeight);
            //glViewport(0, 0, 1280, 720);


            Core::Game* game = editor->getGamePtr();

            glBindFramebuffer(GL_FRAMEBUFFER, mMapFBO);
            glClearColor(bgCol.r, bgCol.g, bgCol.b, bgCol.a);
            glClear(GL_COLOR_BUFFER_BIT);
            editor->getGamePtr()->renderDefaultMapPage(); // Render Game with new viewport size
            handleClick();

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ImVec2 uv0 = ImVec2(0.5f - (0.5f*canvas_size.x)/mapWidth, 0.5f - (0.5f*canvas_size.y)/mapHeight);
            ImVec2 uv1 = ImVec2(0.5f + (0.5f*canvas_size.x)/mapWidth, 0.5f + (0.5f*canvas_size.y)/mapHeight);
            // Use these for stretchy MapView
            //ImVec2 uv0 = ImVec2(0, 1);
            //ImVec2 uv1 = ImVec2(1, 0);

            ImGui::Image((ImTextureID) mMapTexCBO, ImVec2(canvas_size.x, canvas_size.y), uv0, uv1);

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}