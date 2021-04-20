#include "windows/ScriptEditor.h"

void ScriptEditor::draw()
{
    // Script editor
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        scriptInfo = false;
        if (ImGui::Begin("Script Editor", &visible))
        {
            ImGui::PushItemWidth(200);
            ImGui::InputText("##scriptName", scriptName, IM_ARRAYSIZE(scriptName));

            if (ImGui::Button("Create Script"))
            {
                if (std::string(scriptName).size() != 0)
                    editor->getGamePtr()->createScript(std::string(scriptName));
                else
                {
                    ImGui::SameLine();
                    ImGui::Text("empty name");
                }
            }
            ImGui::Text("Current Script: %s", editor->getCurrentComponentList()[CUR_SCRIPT].c_str());
            if (ImGui::BeginListBox("", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                // script list
                std::vector<Core::Script> *t = editor->getGamePtr()->getScriptsList();
                // Set default selected entity to be the first in the entity list
                if (editor->getCurrentComponentList()[CUR_SCRIPT] == "No Component Selected" && t->size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_SCRIPT] = editor->getGamePtr()->getScriptsList()->at(0).getScriptName();
                }

                for (int i = 0; t != nullptr && i < t->size(); i++)
                {
                    // script
                    auto s = t->at(i);
                    bool is_selected = (currScriptId == i);
                    if (ImGui::Selectable(s.getScriptName().c_str(), is_selected))
                    {
                        currScriptId = i;
                        editor->getCurrentComponentList()[CUR_SCRIPT] = s.getScriptName().c_str();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }
            //ImGui::SameLine();
            if (ImGui::Button("Delete Script"))
            {
            }
            if (ImGui::Button("Link Script"))
            {
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Information"))
            {
                scriptInfo = true;
            }
        }

        ImGui::End();
    }
}