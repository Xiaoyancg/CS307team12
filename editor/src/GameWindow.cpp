#include "GameWindow.h"

#include "Editor.h"



void GameWindow::draw()
{
    if (isVisible())
    {
        // possibly implement a new function here for readability purposes
        if (editor->getGamePtr() != nullptr)
        {
            // set the windows default size
            ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

            // the game view window itself
            ImGui::Begin("Game View", &visible);
            updateWindowFocus();

            //ImVec2 dims = ImGui::GetWindowSize();

            // Get size of drawable space on the window, instead of the entire size of the window
            ImVec2 canvas_size = ImGui::GetContentRegionAvail();

            glViewport(0, 0, editor->getGamePtr()->width, editor->getGamePtr()->height); // Set viewport to the Game dimensions

            editor->getGamePtr()->render(); // Render Game with new viewport size

            //glViewport(0, 0, (int)dims.x, (int)dims.y); // Reset viewport size
            //ImGui::Image((void *)(*texcbo), ImVec2(dims.x, dims.y), ImVec2(0, 1), ImVec2(1, 0));

            glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
            ImGui::Image((void *)(*editor->getTexCBO()), ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::End();
            ImGui::PopStyleVar();
        }
    }
}
