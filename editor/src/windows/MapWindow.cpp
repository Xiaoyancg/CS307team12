#include "windows/MapWindow.h"

ImVec2 prevClick(0,0);

// handleClick will return a pointer to a Tile on the current Map based on whether
// a click has occured, or nullptr 
Core::Tile* MapWindow::handleClick() {
    // Calculate the min/max coords of the MapView window
    // min/max are used to check for click detection within the coordinates of the window
    ImVec2 min = ImGui::GetWindowPos();
    ImVec2 max = min;
    min.y += ImGui::GetWindowContentRegionMin().y; // Get y value of window past the title bar
    max.x += ImGui::GetWindowContentRegionMax().x; // Get max x value of MapView
    max.y += ImGui::GetWindowContentRegionMax().y; // Get max y value of MapView

    // Get width/height and the center coordinates of the ImGui drawable space.
    // Clicks are sent to the core based on their distance from the center.
    ImVec2 canvasDimensions(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x,
                            ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
    glm::ivec2 center(canvasDimensions.x / 2, canvasDimensions.y / 2);

    //////////// Mouse handling within the MapView window ////////////
    // If mouse left is currently pressed and the MapView is focused
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowFocused()) {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y)) {
            // Get x and y of the click based on their distance from the center of the MapView screen.
            // This lets MapPage::getTileFromClick() understand the click as a point in its coordinate system without orthographic projection / zoom.
            // The orthographic projection is calculated on the click later in MapPage::getTileFromClick() based on the current Map's Camera.
            float x = (click_pos.x - min.x);
            float y = canvasDimensions.y - (click_pos.y - min.y);
            glm::ivec2 diff(x - center.x, y - center.y);

            // NOTE: Right now the code just gets the clicked Tile and sets its sprite ID to 1. It should probably do something else...
            Core::Tile* clicked_tile = editor->getGamePtr()->getDefaultMapPage()->getTileFromClick(diff.x, diff.y);

            if (clicked_tile) {
                // TODO: DO something else here
                clicked_tile->setSpriteID(1);
                return clicked_tile;
            }
        }
    }
    // Handle mouse wheel zoom
    else if (ImGui::GetIO().MouseWheel != 0.0f) {
        float wheel = ImGui::GetIO().MouseWheel;
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y)) {
            // The click is valid within the MapView window.
            Core::Camera* cam = editor->getCurrentMap()->getCamera();
            if (wheel > 0) {
                    cam->setZoom(cam->getZoom() - .05); // Zoom in 
            }
            else if (wheel < 0) {
                cam->setZoom(cam->getZoom() + .05); // Zoom out
            }
        }
    }
    // Handle right-click drag
    else if (ImGui::IsMouseDown(1) && ImGui::IsMouseDragging(1) && ImGui::IsWindowFocused()) {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
            // The click is valid within the MapView window.
            Core::Camera* cam = editor->getCurrentMap()->getCamera();
            // Move the position to the offset of the drag. Using ImGui::GetMouseDrag messed things up here, so I'm doing it this way
            cam->offsetPosition(glm::ivec2(-(click_pos.x - prevClick.x), click_pos.y - prevClick.y));
            // Set previous click position to calculate next drag
            prevClick.x = click_pos.x;
            prevClick.y = click_pos.y;
    }
    // Handle initial right-click
    else if (ImGui::IsMouseDown(1) && ImGui::IsWindowFocused()) {
        ImVec2 click_pos = ImGui::GetMousePos();
        // Set prevClick in case of dragging
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y)) {
            prevClick.x = click_pos.x;
            prevClick.y = click_pos.y;
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
            ImGui::SetWindowSize(ImVec2(mapWidth, mapHeight)); // Make enough room for entire map on window

            Core::MapPage* mapPage = editor->getGamePtr()->getDefaultMapPage();
            glm::vec4& bgCol = mapPage->GetBackgroundColor();


            glBindTexture(GL_TEXTURE_2D, mMapTexCBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mapWidth, mapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float bgColArr[] = { bgCol.r, bgCol.g, bgCol.b, bgCol.a };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bgColArr);
            glBindTexture(GL_TEXTURE_2D, 0);
            glViewport(0, 0, (int)mapWidth, (int)mapHeight);

            Core::Game* game = editor->getGamePtr();

            glBindFramebuffer(GL_FRAMEBUFFER, mMapFBO);
            glClearColor(bgCol.r, bgCol.g, bgCol.b, bgCol.a);
            glClear(GL_COLOR_BUFFER_BIT);

            handleClick(); // Handle clicks within the MapView window
            editor->getGamePtr()->renderDefaultMapPage(); // Render Game with new viewport size

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //ImVec2 uv1 = ImVec2(0.5f - (0.5f * canvas_size.x) / mapWidth, 0.5f - (0.5f * canvas_size.y) / mapHeight);
            //ImVec2 uv0 = ImVec2(0.5f + (0.5f * canvas_size.x) / mapWidth, 0.5f + (0.5f * canvas_size.y) / mapHeight);
            // Use these for stretchy MapView
            // Stretching isn't allowed because the window size isn't changable. I just needed these uv values because ^ those flipped the screen in some way
            ImVec2 uv0 = ImVec2(0, 1);
            ImVec2 uv1 = ImVec2(1, 0);

            ImGui::Image((ImTextureID) mMapTexCBO, ImVec2(canvas_size.x, canvas_size.y), uv0, uv1);

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}