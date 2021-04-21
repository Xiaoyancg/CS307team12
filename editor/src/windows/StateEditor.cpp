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
        if (ImGui::Begin("State Editor", &visible))
        {
            ImGui::Text("Enter name:");
            ImGui::SameLine();
            ImGui::PushItemWidth(200);
            ImGui::InputText("##state_name", state_name, IM_ARRAYSIZE(state_name));
            if (ImGui::Button("Create Score Tracker"))
            {

            }
            ImGui::SameLine();
            HelpMarker("A score tracker allows for in-game scores to be updated based on event outcomes.");
            if (ImGui::Button("Create Entity Property"))
            {

            }
            ImGui::SameLine();
            HelpMarker("Entity properties include entity characteristics like HP and speed that can be affected by in-game events such as score changes or collisions.");
            //ImGui::Checkbox("Controllable", &controllable);
        }
        ImGui::End();
    }
}