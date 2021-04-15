#include "windows/LogicEditor.h"
#include "UndoRedo.h"

void LogicEditor::draw()
{
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char logic_name[128] = "";
        static char logicIDInput[128] = "";
        logicInfo = false;
        if (ImGui::Begin("Logic Editor", &visible))
        {
            ImGui::Text("Current Logic: ");
            if (ImGui::BeginListBox("", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                ImGui::EndListBox();
            }

            if (ImGui::Button("Delete Logic"))
            {
            
            }

            if (ImGui::Button("Show Information"))
            {
                logicInfo = true;
            }

            if (logicInfo)
            {
                ImGui::OpenPopup("Logic Information");
                logicInfo = false;
            }

            // Logic information popup
            if (ImGui::BeginPopup("Logic Information"))
            {
                ImGui::Text("Logic Name: ");
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}
