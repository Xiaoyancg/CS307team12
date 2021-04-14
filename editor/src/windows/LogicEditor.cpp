#include "windows/LogicEditor.h"
#include "UndoRedo.h"

// TODO: Make logic ID input turn all negative IDs to -1?

void LogicEditor::draw()
{
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char logic_name[128] = "";
        static int logicIDInput = -1;
        logicInfo = false;
        if (ImGui::Begin("Logic Editor", &visible))
        {
            // Set input widget widths to 200
            ImGui::PushItemWidth(200);

            ImGui::Text("Logic Name:");
            ImGui::SameLine();
            ImGui::InputText("##logic_name", logic_name, IM_ARRAYSIZE(logic_name));

            ImGui::Text("ID:        ");
            ImGui::SameLine();
            ImGui::InputInt("##logic_id", &logicIDInput);

            ImGui::Text("Type:      ");
            ImGui::SameLine();
            const char* items[] = { "Key", "Custom" };
            static int item_current_idx = 0; // Here we store our selection data as an index.
            const char* combo_label = items[item_current_idx];
            if (ImGui::BeginCombo("##logic_type", items[item_current_idx]))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        item_current_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::PopItemWidth();

            ImGui::Text("Current Logic: ");
            if (ImGui::BeginListBox(" ", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                ImGui::EndListBox();
            }
            if (ImGui::Button("Show Information"))
            {
                logicInfo = true;
            }

            if (ImGui::Button("Create"))
            {

            }
            ImGui::SameLine();
            if (ImGui::Button("Update"))
            {

            }
            ImGui::SameLine();
            if (ImGui::Button("Delete"))
            {
            
            }

            // Open logic info popup when "Show Information" is clicked
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
