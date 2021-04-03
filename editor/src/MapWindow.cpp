#include "MapWindow.h"
#include "Editor.h"


void MapWindow::draw()
{
    // possibly implement a new function here for readability purposes
    if (editor->getCurrentMap() != nullptr)
    {
        Core::Map* currMap = editor->getCurrentMap();
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        // the game view window itself
        ImGui::Begin("Map View", &visible);

        // Get size of drawable space on the window, instead of the entire size of the window
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        glm::ivec2 dims = currMap->getDimensions();
        int tileSize = currMap->getTileSize();
        //glViewport(0, 0, dims.x * tileSize, dims.y * tileSize);
        glViewport(0, 0, 1000, 1000);

        editor->getGamePtr()->renderDefaultMapPage(); // Render Game with new viewport size

        glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
        ImGui::Image((void *)(*editor->getMapTexCBO()), ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();
    }
}