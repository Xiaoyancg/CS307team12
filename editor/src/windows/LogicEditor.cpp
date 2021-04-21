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
        static int current_logic = 0;
        logicInfo = false;
        Core::Game* game = editor->getGamePtr();

        // For "key type" logic
        char key_code[2] = "";
        static int key_idx = 0;

        char target_script_list[128] = "";
        
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
            // The order of these options should be preserved for now
            // They are somewhat hardcoded based on the Core::SignalType enum
            const char* logic_types[] = { "Custom", "Key" };
            static int logic_type_idx = 0; // Here we store our selection data as an index.
            const char* combo_label = logic_types[logic_type_idx];
            if (ImGui::BeginCombo("##logic_type", logic_types[logic_type_idx]))
            {
                for (int n = 0; n < IM_ARRAYSIZE(logic_types); n++)
                {
                    const bool is_selected = (logic_type_idx == n);
                    if (ImGui::Selectable(logic_types[n], is_selected))
                        logic_type_idx = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ImGui::Separator();

            // Render different UI options based on logic type selected
            switch (logic_type_idx)
            {
                
                case 0: // Custom type
                {
                    ImGui::Text("Target Script List:");
                    ImGui::SameLine();
                    ImGui::InputText("##target_script_list", target_script_list, IM_ARRAYSIZE(target_script_list));
                }
                    break;
                case 1: // Key type
                {
                    ImGui::Text("Key Code:  ");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(25);
                    ImGui::InputText("##key_code", key_code, IM_ARRAYSIZE(key_code));
                    ImGui::PopItemWidth();

                    ImGui::Text("Key Type:  ");
                    ImGui::SameLine();
                    const char* key_types[] = { "Pressed", "Released" };
                    const char* combo_label = key_types[key_idx];
                    if (ImGui::BeginCombo("##key_type", key_types[key_idx]))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(key_types); n++)
                        {
                            const bool is_selected = (key_idx == n);
                            if (ImGui::Selectable(key_types[n], is_selected))
                                key_idx = n;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Text("Target Script List:");
                    ImGui::SameLine();
                    ImGui::InputText("##target_script_list", target_script_list, IM_ARRAYSIZE(target_script_list));
                }
                break;
            }

            ImGui::PopItemWidth();
            ImGui::Separator();
             
            ImGui::Text("Current Logic: %s", editor->getCurrentComponentList()[CUR_LOGIC].c_str());
            if (ImGui::BeginListBox("##logic_listbox", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                auto logic_list = *game->getLogicList();
                // Set default selected logic to be the first in the logic list
                if (editor->getCurrentComponentList()[CUR_LOGIC] == "No Component Selected" && logic_list.size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_LOGIC] = logic_list[0].getLogicName();
                }

                for (int i = 0; i < logic_list.size(); i++)
                {
                    auto logic = logic_list[i];
                    const bool is_selected = (current_logic == i);
                    if (ImGui::Selectable(logic.getLogicName().c_str(), is_selected))
                    {
                        current_logic = i;
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

            if (ImGui::Button("Show Information"))
            {
                logicInfo = true;
            }

            if (ImGui::Button("Create"))
            {
                if (strlen(logic_name) != 0)
                {
                    std::string lname = logic_name;
                    Core::Logic * new_logic = game->createLogic();
                    new_logic->setLogicName(std::string(logic_name));
                    new_logic->setLogicId(logicIDInput);
                    switch (logic_type_idx)
                    {
                        case 0: // Custom type
                            new_logic->setSignalType(Core::SignalType::Custom);
                            break;
                        case 1: // Key type
                            new_logic->setSignalType(Core::SignalType::Key);
                            break;
                    }

                    //UNDO
                    /*std::string pname = page_name;
                    auto action = [this, pname]() {
                        editor->getGamePtr()->createMenuPage(pname);
                    };
                    auto restore = [this, pname]() {
                        editor->getGamePtr()->deletePage(pname);
                    };
                    pushAction(action, restore);
                    action();
                    //ENDUNDO
                    */

                    // memset to clear the buffer after use
                    memset(logic_name, 0, 128);
                    logicIDInput = -1;
                }
                
            }
            ImGui::SameLine();
            if (ImGui::Button("Update"))
            {

            }
            ImGui::SameLine();
            std::string currentLogicName = editor->getCurrentComponentList()[CUR_LOGIC];
            if (ImGui::Button("Delete") && currentLogicName != "No Component Selected")
            {
                int idx = -1;
                auto logic_list = *game->getLogicList();
                for (auto logic : logic_list) 
                {
                    if (logic.getLogicName() == currentLogicName)
                    {
                        game->deleteLogic(logic.getLogicId());
                    }
                }

                // memset to clear the buffer after use
                memset(logic_name, 0, 128);
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
