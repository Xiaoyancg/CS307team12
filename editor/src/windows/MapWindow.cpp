#include "windows/MapWindow.h"
#include "windows/MapEditor.h"

ImVec2 prevClick(0, 0);

// handleClick will return a pointer to a Tile on the current Map based on whether
// a click has occurred, or nullptr
Core::Tile *MapWindow::handleClick()
{
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
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImGui::GetIO();
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        lastTile = nullptr;
    }
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowFocused()) {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y))
        {
            // Get x and y of the click based on their distance from the center of the MapView screen.
            // This lets MapPage::getTileFromClick() understand the click as a point in its coordinate system without orthographic projection / zoom.
            // The orthographic projection is calculated on the click later in MapPage::getTileFromClick() based on the current Map's Camera.
            glm::vec2 windowClick = glm::vec2(
                (click_pos.x - min.x) / canvasDimensions.x,
                1 - (click_pos.y - min.y) / canvasDimensions.y
            );

            // NOTE: Right now the code just gets the clicked Tile and sets its sprite ID to 1. It should probably do something else...
            Core::Tile *clicked_tile = editor->getCurrentMap()->getTileFromClick(cam, windowClick.x, windowClick.y);

            if (clicked_tile && clicked_tile != lastTile)
            {
                MapEditor* mapEditor = static_cast<MapEditor*>(editor->getWindowList()[MAPEDITOR]);
                switch (mapEditor->getEditMode()) {
                    case EditMode::Collision:
                        clicked_tile->setSolid(!clicked_tile->isSolid());
                        break;
                    case EditMode::Sprite:
                        clicked_tile->setSpriteID(mapEditor->getSelectedSpriteID());
                        break;
                }
                lastTile = clicked_tile;
                return clicked_tile;
            }
        }
    }
    // Handle mouse wheel zoom
    if (ImGui::GetIO().MouseWheel != 0.0f)
    {
        float wheel = ImGui::GetIO().MouseWheel;
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y))
        {
            // The click is valid within the MapView window.
            if (wheel > 0)
            {
                cam->setZoom(cam->getZoom() + 0.05f); // Zoom in
            }
            else if (wheel < 0)
            {
                cam->setZoom(cam->getZoom() - 0.05f); // Zoom out
            }
        }
    }
    // Handle middle-click drag
    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle) && ImGui::IsMouseDragging(ImGuiMouseButton_Middle) && ImGui::IsWindowFocused())
    {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        // The click is valid within the MapView window.
        // Move the position to the offset of the drag. Using ImGui::GetMouseDrag messed things up here, so I'm doing it this way
        cam->offsetPosition(glm::vec2(-(click_pos.x - prevClick.x), click_pos.y - prevClick.y));
        // Set previous click position to calculate next drag
        prevClick.x = click_pos.x;
        prevClick.y = click_pos.y;
    }
    // Handle initial right-click
    if (ImGui::IsMouseDown(ImGuiMouseButton_Middle) && ImGui::IsWindowFocused())
    {
        ImVec2 click_pos = ImGui::GetMousePos();
        // Set prevClick in case of dragging
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y))
        {
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
            glm::ivec2 icanvas_size = glm::ivec2((int)canvas_size.x, (int)canvas_size.y);
            glm::ivec2 dims = currMap->getDimensions();
            int tileSize = currMap->getTileSize();
            int mapWidth = (dims.x + 1) * tileSize;
            int mapHeight = (dims.y + 1) * tileSize;

            // set texture size to map dimensions so that we don't get quality loss
            // --initially I thought about only rendering the visible part of the map
            // --but that won't work without a shader rework
            glBindTexture(GL_TEXTURE_2D, mMapTexCBO);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, icanvas_size.x, icanvas_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            cam->setDimensions(icanvas_size.x, icanvas_size.y);
            //cam->setPosition(mapWidth/2.0f, mapHeight/2.0f);
            cam->use();

            // render the map
            glBindFramebuffer(GL_FRAMEBUFFER, mMapFBO);
            // and make sure the viewport knows the size of the output texture
            glm::vec4 bgCol(0.1f, 0.9f, 0.59f, 1.0f);
            glViewport(0, 0, icanvas_size.x, icanvas_size.y);
            glClearColor(bgCol.r, bgCol.g, bgCol.b, bgCol.a);
            glClear(GL_COLOR_BUFFER_BIT);
            currMap->render();
            if (static_cast<MapEditor*>(editor->getWindowList()[MAPEDITOR])->getEditMode() == EditMode::Collision) {
                currMap->renderCollisionHelper(mTintTexture);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // draw the rendered image to the window
            ImGui::Image((ImTextureID)mMapTexCBO, ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

            // Handle clicks within the MapView window
            handleClick();

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}