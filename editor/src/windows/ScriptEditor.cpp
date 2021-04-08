#include "windows/ScriptEditor.h"


void ScriptEditor::draw() {
	// Script editor
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char script_name[128] = "";
        bool script_info = false;
        if (ImGui::Begin("Script Editor", &visible))
        {
            ImGui::PushItemWidth(200);
            ImGui::InputText(" ", script_name, IM_ARRAYSIZE(script_name));
            if (ImGui::Button("Create New Script"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Script"))
            {
            }
            if (ImGui::Button("Link This Script"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Script Information"))
            {
                script_info = true;
            }
        }

        ImGui::End();
    }
}