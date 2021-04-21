#include "windows/LogicEditor.h"
#include "UndoRedo.h"

// TODO: Make logic ID input turn all negative IDs to -1?

void LogicEditor::draw()
{
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        Core::Game *game = editor->getGamePtr();

        // For "key type" logic

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

            ImGui::Text("Signal Type: ");
            ImGui::SameLine();
            // The order of these options should be preserved for now
            // They are somewhat hardcoded based on the Core::SignalType enum
            if (ImGui::BeginCombo("##signal_type", signal_type_strings[signal_type_idx]))
            {
                for (int n = 0; n < IM_ARRAYSIZE(signal_type_strings); n++)
                {
                    bool is_selected = (signal_type_idx == n);
                    if (ImGui::Selectable(signal_type_strings[n], is_selected))
                    {
                        signal_type_idx = n;
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::Text("Script Type: ");
            ImGui::SameLine();
            // The order of these options should be preserved for now
            // They are somewhat hardcoded based on the Core::ScriptType enum
            if (ImGui::BeginCombo("##script_type", script_type_strings[script_type_idx]))
            {
                for (int n = 0; n < IM_ARRAYSIZE(script_type_strings); n++)
                {
                    bool is_selected = (script_type_idx == n);
                    if (ImGui::Selectable(script_type_strings[n], is_selected))
                    {
                        script_type_idx = n;
                    }
                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::TreeNode("Target Script List"))
            {
                for (int i = 0; i < targetScriptList.size(); i++)
                {
                    ImGui::Text("%d", targetScriptList.at(i));
                    ImGui::SameLine();
                    ImGui::PushItemWidth(80);
                    std::string deletebtnName("delete " + std::to_string(i));
                    if (ImGui::Button(deletebtnName.c_str()))
                    {
                        targetScriptList.erase(targetScriptList.begin() + i);
                    }
                    ImGui::PopItemWidth();
                }
                ImGui::Text("new target Script");
                ImGui::SameLine();
                ImGui::InputInt("##newTargetScript", &newTargetScript);
                ImGui::SameLine();
                if (ImGui::Button("Add"))
                {
                    targetScriptList.push_back(newTargetScript);
                }
                ImGui::TreePop();
            }

            ImGui::Separator();

            // Render different UI options based on logic type selected
            switch (signal_type_idx)
            {
            case 0: // Custom type
            {
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
                            // imgui use SDL_SCANCODE
                            keyCode = SDL_GetKeyFromScancode((SDL_Scancode)i);
                            strcpy_s(keyCodeString, (SDL_GetKeyName(keyCode)));
                            break;
                        }
                    }
                }
                ImGui::PopItemWidth();

                ImGui::Text("Key Type:  ");
                ImGui::SameLine();
                const char *key_types[] = {"KeyDown", "KeyUp"};
                if (ImGui::BeginCombo("##key_type", key_types[keyTypeIdx]))
                {

                    for (int n = 0; n < IM_ARRAYSIZE(key_types); n++)
                    {
                        const bool is_selected = (keyTypeIdx == n);
                        if (ImGui::Selectable(key_types[n], is_selected))
                        {
                            keyTypeIdx = n;
                            keyType = keyTypeIdx == 0 ? SDL_KEYDOWN : SDL_KEYUP;
                        }
                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            break;
            }

            ImGui::PopItemWidth();
            ImGui::Separator();

            // Find the current logic in the logic list
            //if (editor->getCurrentComponentList()[CUR_LOGIC] != "No Component Selected" &&
            //    current_logic->getLogicName() != editor->getCurrentComponentList()[CUR_LOGIC])
            //{
            //    for (auto logic : *game->getLogicList())
            //    {
            //        if (logic.getLogicName() == editor->getCurrentComponentList()[CUR_LOGIC])
            //        {
            //            current_logic = logic;
            //        }
            //    }
            //}
            if (current_logic != nullptr)
            {
                // Display info about current logic
                ImGui::Text("Current Logic: %s", editor->getCurrentComponentList()[CUR_LOGIC].c_str());
                if (current_logic->getSignalType() == Core::SignalType::Custom)
                {
                    ImGui::Text("Logic Type: Custom");
                }
                else if (current_logic->getSignalType() == Core::SignalType::Key)
                {
                    ImGui::Text("Logic Type: Key");
                }
            }
            // List of all the logics in logic list
            if (ImGui::BeginListBox("##logic_listbox", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                auto logic_list = game->getLogicList();
                // Set default selected logic to be the first in the logic list
                if (editor->getCurrentComponentList()[CUR_LOGIC] == "No Component Selected" && logic_list->size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_LOGIC] = logic_list->at(0).getLogicName();
                }

                for (int i = 0; i < logic_list->size(); i++)
                {
                    auto &logic = logic_list->at(i);
                    const bool is_selected = (current_logic_idx == i);
                    std::string selectable_name = logic.getLogicName() + " (" + std::to_string(logic.getLogicId()) + ")";
                    if (ImGui::Selectable(selectable_name.c_str(), is_selected))
                    {
                        current_logic_idx = i;
                        editor->getCurrentComponentList()[CUR_LOGIC] = logic.getLogicName();
                        current_logic = &(logic_list->at(i));
                        logicIDInput = current_logic->getLogicId();
                        std::strcpy(logic_name, current_logic->getLogicName().c_str());
                        script_type_idx = (int)(current_logic->getScriptType());
                        signal_type_idx = (int)(current_logic->getSignalType());
                        targetScriptList = current_logic->getTargetScriptList();
                        switch (signal_type_idx)
                        {
                        case 0:
                            break;
                        case 1:
                            keyCode = current_logic->getLogic().keyLogic.getKey();
                            keyType = current_logic->getLogic().keyLogic.getKeyType();
                            keyTypeIdx = keyType == SDL_KEYDOWN ? 0 : 1;
                            std::strcpy(keyCodeString, SDL_GetKeyName(keyCode));
                        }
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

                    int ltype = signal_type_idx;
                    Core::Logic *new_logic = editor->getGamePtr()->createLogic();
                    new_logic->setLogicName(lname);
                    new_logic->setLogicId(lID);
                    new_logic->setSignalType((Core::SignalType)signal_type_idx);
                    new_logic->setScriptType((Core::ScriptType)script_type_idx);
                    new_logic->setTargetScriptList(targetScriptList);
                    switch (ltype)
                    {
                    case 0: // Custom type
                        break;
                    case 1: // Key type
                        new_logic->setLogic(Core::LogicUnion(Core::LogicKey(keyCode, keyType)));
                        break;
                    }

                    //auto action = [this, lname, lID, ltype]() {
                    //    Core::Logic *new_logic = editor->getGamePtr()->createLogic();
                    //    new_logic->setLogicName(lname);
                    //    new_logic->setLogicId(lID);
                    //    switch (ltype)
                    //    {
                    //    case 0: // Custom type
                    //        new_logic->setSignalType(Core::SignalType::Custom);
                    //        break;
                    //    case 1: // Key type
                    //        new_logic->setSignalType(Core::SignalType::Key);
                    //        break;
                    //    }
                    //};

                    //auto restore = [this, lname]() {
                    //    // Find the logic to delete
                    //    auto logic_list = *editor->getGamePtr()->getLogicList();
                    //    for (auto logic : logic_list)
                    //    {
                    //        if (logic.getLogicName() == lname)
                    //        {
                    //            editor->getGamePtr()->deleteLogic(logic.getLogicId());
                    //        }
                    //    }
                    //};

                    //pushAction(action, restore);
                    //action();

                    // memset to clear the buffer after use
                    memset(logic_name, 0, 128);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Update") && current_logic != nullptr)
            {
                current_logic->setLogicName(logic_name);
                current_logic->setLogicId(logicIDInput);
                current_logic->setSignalType((Core::SignalType)signal_type_idx);
                current_logic->setScriptType((Core::ScriptType)script_type_idx);
                current_logic->setTargetScriptList(targetScriptList);
                switch (signal_type_idx)
                {
                case 0: // Custom type
                    break;
                case 1: // Key type
                    current_logic->setLogic(Core::LogicUnion(Core::LogicKey(keyCode, keyType)));
                    break;
                }
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
