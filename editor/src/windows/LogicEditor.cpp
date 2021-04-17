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
        if (ImGui::Begin("Logic Editor", &visible))
        {
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

            if (ImGui::Button("Show Information"))
            {
                logicInfo = true;
            }

            if (ImGui::Button("Create"))
            {
                if (strlen(logic_name) != 0)
                {
                    Core::Logic * new_logic = game->createLogic();
                    new_logic->setLogicName(std::string(logic_name));

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
                }
                
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
