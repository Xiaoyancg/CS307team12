#include "windows/MenuPageEditor.h"
#include "MenuPage.h"

// handleClick will return a pointer to a Tile on the current Map based on whether
// a click has occurred, or nullptr
void MenuPageEditor::handleClick()
{
    /*
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
            Core::Tile* clicked_tile = editor->getCurrentMap()->getTileFromClick(cam, windowClick.x, windowClick.y);

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
            }             else if (wheel < 0)
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
    */
    //return nullptr;
}

void MenuPageEditor::draw() {
	Core::Game* game = editor->getGamePtr();

	static char entry_text[128] = "";
	static float size = 1.0f;
	static int loc_x = 0;
	static int loc_y = 0;
	static int dim_x = 0;
	static int dim_y = 0;

	if (visible && game)
	{
		Core::MenuPage* menuPage;
		if (editor->getCurrentComponentList()[CUR_PAGE] != "" && (menuPage = dynamic_cast<Core::MenuPage*>(editor->getGamePtr()->getCurrPage())))
		{
		

			// Menu editor popup
			ImGui::Begin("Menu Editor", &visible);

			// Text
			ImGui::PushItemWidth(200);
			ImGui::Text("Enter Item Text:");
			ImGui::InputText("  ", entry_text, IM_ARRAYSIZE(entry_text));


			// Size
			ImGui::Text("Text Size: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderFloat("##size", &size, 0.0f, 3.0f);


			// Location
			ImGui::Text("Location: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_x", &loc_x, -Core::Game::width/2, Core::Game::width/2);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_y", &loc_y, -Core::Game::height / 2, Core::Game::height/2);

			// Create text box
			if (ImGui::Button("Create Text Entry")) {
				game->createMenuTextBox(entry_text, size, glm::vec2(loc_x, loc_y), glm::vec3(.24, .52, .89));
			}

			ImGui::Text("Dimensions: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_x", &dim_x, 0, Core::Game::width / 2);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_y", &dim_y, 0, Core::Game::height / 2);


			// Create text box
			if (ImGui::Button("Create Button Entry")) {
				game->createMenuButton(entry_text, size, glm::vec3(1, 1, 1), glm::vec2(loc_x, loc_y), glm::vec2(dim_x, dim_y), glm::vec3(.24, .52, .89));
			}

			ImGui::End();

            handleClick();
		}
	}
}
