#include "windows/StateEditor.h"
#include "UndoRedo.h"
#include "HelpMarker.h"

void StateEditor::draw() {
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        static char state_name[128] = "";
        static bool on_collision = false;
        static bool on_property_change = false;
        if (ImGui::Begin("State Editor", &visible))
        {
            if (ImGui::Button("Create Score Tracker"))
            {

            }
            ImGui::SameLine();
            HelpMarker("A score tracker allows for in-game scores to be updated based on event outcomes.");
            if (ImGui::Button("Create Entity Property"))
            {

            }
            ImGui::SameLine();
            HelpMarker("Entity properties include characteristics such as health and speed that can be affected by in-game events.");
            ImGui::NewLine();
            ImGui::Checkbox("Change State On Collision", &on_collision);
            ImGui::SameLine();
            HelpMarker("Update a state if a collision between two entities occurs.");
            ImGui::Checkbox("Change State On Existing State Change", &on_property_change);
            ImGui::SameLine();
            HelpMarker("Update an existing state based on the outcome of another state (e.g. change the game's score if an entity's health is depleted).");
        }
        ImGui::End();
    }
}