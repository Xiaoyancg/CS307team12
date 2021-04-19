#include "windows/SignalEditor.h"
#include "UndoRedo.h"


void SignalEditor::draw() {
    if (visible)
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        static char signal_name[128] = "";
        static int current_item = 0;
        //TODO: update the following with signal options
        const char* signal_options[] = { "Test1", "Test2" };
        bool signal_info = false;
        static int sigid = 0;
        //TODO: update the following array once we can get a list of logic types
        const char* target_logics[] = { "Target Logic 1", "Target Logic 2" };
        if (ImGui::Begin("Signal Editor", &visible))
        {
            ImGui::PushItemWidth(200);
            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::InputText("##signal_name", signal_name, IM_ARRAYSIZE(signal_name));
            ImGui::Text("ID:  ");
            ImGui::SameLine();
            ImGui::InputInt("##signal_id", &sigid);
            ImGui::Text("");
            ImGui::Text("Type:");

            if (ImGui::BeginListBox("##signal_type", ImVec2(200, 2 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < IM_ARRAYSIZE(signal_options); n++)
                {
                    const bool is_selected = (current_item == n);
                    if (ImGui::Selectable(signal_options[n], is_selected))
                        current_item = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }

            ImGui::Text("\nTarget Logic:");
            if (ImGui::BeginListBox("##target_logic", ImVec2(200, 2 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < IM_ARRAYSIZE(target_logics); n++)
                {
                    const bool is_selected = (current_item == n);
                    if (ImGui::Selectable(target_logics[n], is_selected))
                        current_item = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            
            ImGui::Text("");
            if (ImGui::Button("Create Signal"))
            {
                if (strlen(signal_name) != 0)
                {
                    if (strcmp(signal_options[current_item], "Test1") == 0)
                    {
                        //create signal type
                    }                     else
                    {
                        //create other signal type
                    }
                }
            }

            ImGui::Text("");
            ImGui::Text("Current Signal: %s", editor->getCurrentComponentList()[CUR_SIGNAL].c_str());
            if (ImGui::BeginListBox("##signal_listbox", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                //logic list will go here
                ImGui::EndListBox();
            }

            if (ImGui::Button("Update Signal"))
            {
                //change signal id/name
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Signal") && editor->getCurrentComponentList()[CUR_PAGE] != "No Component Selected")
            {
                //delete signal
            }
            if (ImGui::Button("Show Information"))
            {
                signal_info = true;
            }
            ImGui::End();
        }
    }
}