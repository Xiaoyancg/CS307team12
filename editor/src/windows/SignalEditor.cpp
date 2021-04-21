#include "windows/SignalEditor.h"
#include "UndoRedo.h"

void SignalEditor::draw() {
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char signal_name[128] = "";
        static int sigid = 0;
        static int current_item = 0;
        static int current_item_2 = 0;

        if (ImGui::Begin("Signal Editor", &visible))
        {
            ImGui::PushItemWidth(200);
            ImGui::Text("Name:");
            ImGui::SameLine();
            ImGui::InputText("##signal_name", signal_name, IM_ARRAYSIZE(signal_name));
            ImGui::Text("ID:  ");
            ImGui::SameLine();
            ImGui::InputInt("##signal_id", &sigid);
            ImGui::Text("Type:");
            ImGui::SameLine();

            //TODO: update the following with signal options
            //The following code allows us to select a signal type from a dropdown.
            const char* signal_types[] = { "Custom", "[PLACEHOLDER]" };
            static int signal_type_idx = 0; 
            const char* combo_label = signal_types[signal_type_idx];
            if (ImGui::BeginCombo("##signal_type", signal_types[signal_type_idx]))
            {
                for (int n = 0; n < IM_ARRAYSIZE(signal_types); n++)
                {
                    const bool is_selected = (signal_type_idx == n);
                    if (ImGui::Selectable(signal_types[n], is_selected))
                        signal_type_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            //The following code will display the list of current logics that have been created by the user.
            //The idea is that the user can select a target logic to bind a signal with.
            ImGui::Separator();
            static int current_logic_idx = 0;
            ImGui::Text("Target Logic:");
            if (ImGui::BeginListBox("##logic_listbox", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                auto logic_list = *editor->getGamePtr()->getLogicList();
                for (int i = 0; i < logic_list.size(); i++)
                {
                    auto logic = logic_list[i];
                    const bool is_selected = (current_logic_idx == i);
                    std::string selectable_name = logic.getLogicName() + " (" + std::to_string(logic.getLogicId()) + ")";
                    if (ImGui::Selectable(selectable_name.c_str(), is_selected))
                    {
                        current_logic_idx = i;
                        editor->getCurrentComponentList()[CUR_LOGIC] = logic.getLogicName();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            //Here, we can create a signal. The user can choose between types of signals.
            if (ImGui::Button("Create Signal"))
            {
                if (strlen(signal_name) != 0)
                {
                    if (strcmp(signal_types[current_item], "Custom") == 0)
                    {
                        Core::Signal * signal = editor->getGamePtr()->createSignal();
                        signal->setSignalName(signal_name);
                        signal->setSignalId(sigid);
                    }                     
                    else
                    {
                        //TODO: create other signal types
                    }
                }
            }

            ImGui::Separator();
            //This code displays the list of current signals.
            ImGui::Text("Current Signal: %s", editor->getCurrentComponentList()[CUR_SIGNAL].c_str());
            static int current_signal_idx = 0;
            if (ImGui::BeginListBox("##signal_listbox", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                auto signal_list = *editor->getGamePtr()->getSignalList();
                // Set default selected signal to be the first in the signal list
                if (editor->getCurrentComponentList()[CUR_SIGNAL] == "No Component Selected" && signal_list.size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_SIGNAL] = signal_list[0].getSignalName();
                }

                for (int i = 0; i < signal_list.size(); i++)
                {
                    auto signal = signal_list[i];
                    const bool is_selected = (current_signal_idx == i);
                    std::string selectable_name = signal.getSignalName() + " (" + std::to_string(signal.getSignalId()) + ")";
                    if (ImGui::Selectable(selectable_name.c_str(), is_selected))
                    {
                        current_signal_idx = i;
                        editor->getCurrentComponentList()[CUR_SIGNAL] = signal.getSignalName();
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            //Update a signal
            if (ImGui::Button("Update"))
            {
                //TODO: change signal id/name
            }

            //Delete a signal
            ImGui::SameLine();
            std::string currentSignalName = editor->getCurrentComponentList()[CUR_SIGNAL];
            if (ImGui::Button("Delete") && editor->getCurrentComponentList()[CUR_PAGE] != "No Component Selected")
            {
                int idx = -1;
                auto signal_list = *editor->getGamePtr()->getSignalList();
                for (auto signal : signal_list)
                {
                    if (signal.getSignalName() == currentSignalName)
                    {
                        editor->getGamePtr()->deleteSignal(signal.getSignalId());
                        editor->showDeleteSuccessPopup();
                    }
                }
            }
        }
        ImGui::End();
    }
}