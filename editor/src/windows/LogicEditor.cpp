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
        static int current_logic_idx = 0;
        Core::Game *game = editor->getGamePtr();
        Core::Logic current_logic;

        // For "key type" logic

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
            const char *logic_types[] = {"Custom", "Key"};
            static int logic_type_idx = 0; // Here we store our selection data as an index.
            const char *combo_label = logic_types[logic_type_idx];
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
                ImGui::InputText("##key_code", keyCodeString, IM_ARRAYSIZE(keyCodeString),
                                 ImGuiInputTextFlags_ReadOnly);
                if (ImGui::IsItemFocused())

                {
                    ImGuiIO &io = ImGui::GetIO();
                    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
                    {
                        if (io.KeysDown[i])
                        {
                            // sdl keycode
                            // 0 is 48 a is 49
                            // a is 97
                            // b is 98

                            // imgui use SDL_SCANCODE
                            // 1 si 30
                            // 0 is 39
                            // a is 4
                            // b is 5
                            strcpy_s(keyCodeString, (SDL_GetKeyName(SDL_GetKeyFromScancode((SDL_Scancode)i))));
                            break;
                        }
                    }
                }
                ImGui::PopItemWidth();

                ImGui::Text("Key Type:  ");
                ImGui::SameLine();
                const char *key_types[] = {"Pressed", "Released"};
                const char *combo_label = key_types[keyType];
                if (ImGui::BeginCombo("##key_type", key_types[keyType]))
                {
                    for (int n = 0; n < IM_ARRAYSIZE(key_types); n++)
                    {
                        const bool is_selected = (keyType == n);
                        if (ImGui::Selectable(key_types[n], is_selected))
                            keyType = n;

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

            // Find the current logic in the logic list
            if (editor->getCurrentComponentList()[CUR_LOGIC] != "No Component Selected" &&
                current_logic.getLogicName() != editor->getCurrentComponentList()[CUR_LOGIC])
            {
                for (auto logic : *game->getLogicList())
                {
                    if (logic.getLogicName() == editor->getCurrentComponentList()[CUR_LOGIC])
                    {
                        current_logic = logic;
                    }
                }
            }

            // Display info about current logic
            ImGui::Text("Current Logic: %s", editor->getCurrentComponentList()[CUR_LOGIC].c_str());
            if (current_logic.getSignalType() == Core::SignalType::Custom)
            {
                ImGui::Text("Logic Type: Custom");
            }
            else if (current_logic.getSignalType() == Core::SignalType::Key)
            {
                ImGui::Text("Logic Type: Key");
            }

            // List of all the logics in logic list
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

            if (ImGui::Button("Create"))
            {
                bool duped_property = false;
                // Enforce unique names and IDs for each logic
                std::string lname = logic_name;
                for (auto logic : *editor->getGamePtr()->getLogicList())
                {
                    if (logic.getLogicName() == lname || logic.getLogicId() == logicIDInput)
                    {
                        duped_property = true;
                    }
                }

                if (strlen(logic_name) != 0 && !duped_property)
                {
                    // UNDO
                    int lID = logicIDInput;
                    int ltype = logic_type_idx;
                    auto action = [this, lname, lID, ltype]() {
                        Core::Logic *new_logic = editor->getGamePtr()->createLogic();
                        new_logic->setLogicName(lname);
                        new_logic->setLogicId(lID);
                        switch (ltype)
                        {
                        case 0: // Custom type
                            new_logic->setSignalType(Core::SignalType::Custom);
                            break;
                        case 1: // Key type
                            new_logic->setSignalType(Core::SignalType::Key);
                            break;
                        }
                    };

                    auto restore = [this, lname]() {
                        // Find the logic to delete
                        auto logic_list = *editor->getGamePtr()->getLogicList();
                        for (auto logic : logic_list)
                        {
                            if (logic.getLogicName() == lname)
                            {
                                editor->getGamePtr()->deleteLogic(logic.getLogicId());
                            }
                        }
                    };

                    pushAction(action, restore);
                    action();

                    // memset to clear the buffer after use
                    memset(logic_name, 0, 128);
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
            }
        }
        ImGui::End();
    }
}
