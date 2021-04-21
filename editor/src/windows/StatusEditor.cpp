#include "windows/StatusEditor.h"
#include "UndoRedo.h"
#include "HelpMarker.h"

void StatusEditor::draw() {
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        static char status_name[128] = "";
        static bool on_collision = false;
        static bool on_property_change = false;
        if (ImGui::Begin("Status Editor", &visible))
        {
            if (ImGui::Button("Create Score Tracker"))
            {

            }
            ImGui::SameLine();
            HelpMarker("A score tracker allows for a score to be maintained and updated based on the outcomes of in-game events.");
            if (ImGui::Button("Create Entity Property"))
            {

            }
            ImGui::SameLine();
            HelpMarker("Entity properties include characteristics such as health and speed that can be affected by in-game events.");
            ImGui::NewLine();
            ImGui::Checkbox("Change Status On Collision", &on_collision);
            ImGui::SameLine();
            HelpMarker("Update a status if a collision between two entities occurs.");
            ImGui::Checkbox("Change Status On Existing Status Change", &on_property_change);
            ImGui::SameLine();
            HelpMarker("Update an existing status based on the outcome of another status (e.g. change the game's score if an entity's health is depleted).");
        }
        ImGui::End();
    }
}