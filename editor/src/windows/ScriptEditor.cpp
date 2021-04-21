#include "windows/ScriptEditor.h"

void ScriptEditor::draw()
{
    // Script editor
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Script Editor", &visible))
        {
            ImGui::PushItemWidth(200.0f);
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
            ImGui::Text("Current Script: \n%s", editor->getCurrentComponentList()[CUR_SCRIPT].c_str());

            if (ImGui::BeginListBox("", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                // script list
                std::vector<Core::Script> *t = editor->getGamePtr()->getScriptsList();
                // Set default selected entity to be the first in the entity list

                for (int i = 0; t != nullptr && i < t->size(); i++)
                {
                    // script
                    auto &s = t->at(i);
                    bool is_selected = (currScriptIdx == i);
                    if (ImGui::Selectable(s.getScriptName().c_str(), is_selected))
                    {
                        currScriptIdx = i;
                        editor->getCurrentComponentList()[CUR_SCRIPT] = s.getScriptName().c_str();
                        currScript_ptr = &(t->at(i));
                        getInfo(currScript_ptr);
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
            if (ImGui::Button("Delete Script") && currScript_ptr != nullptr)
            {
                editor->getGamePtr()->deleteScript(currScript_ptr->getScriptId());
                currScriptIdx = -1;
                currScript_ptr = nullptr;
                editor->getCurrentComponentList()[CUR_SCRIPT] = "No Component Selected";
            }
            if (ImGui::Button("Show/Hide Information"))
            {
                scriptInfo = !scriptInfo;
            }

            // info
            if (currScript_ptr != nullptr && scriptInfo)
            {
                ImGui::Text("id: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(170.0f);
                ImGui::InputInt(" ", &scriptId);
                ImGui::PopItemWidth();
                ImGui::ListBox("Script Type", &type, typeNameList, 1);
                switch (currScript_ptr->getScriptType())
                {
                case Core::ScriptType::Custom:
                    customWidgets();
                    break;

                default:
                    break;
                }
            }

            if (ImGui::Button("Update Script") && currScript_ptr != nullptr)
            {
                switch ((Core::ScriptType)type)
                {
                case Core::ScriptType::Custom:
                    // args's order is inverse
                    currScript_ptr->updateScript((Core::ScriptType)type, scriptId, std::string(scriptName),
                                                 removeTargetScriptList, removeTargetLogicList, removeTargetSignalList,
                                                 addTargetScriptList, addTargetLogicList, addTargetSignalList);
                    break;

                default:
                    break;
                }
            }
        }

        ImGui::End();
    }
}
void ScriptEditor::listTreeNode(std::string listName, int listType)
{
    std::vector<int> *targetList;
    int *newItemId;
    switch (listType)
    {
    case 0:
        targetList = &addTargetSignalList;
        newItemId = &newAddSignal;
        break;
    case 1:
        targetList = &addTargetLogicList;
        newItemId = &newAddLogic;
        break;
    case 2:
        targetList = &addTargetScriptList;
        newItemId = &newAddScript;
        break;
    case 3:
        targetList = &removeTargetSignalList;
        newItemId = &newRemoveSignal;
        break;
    case 4:
        targetList = &removeTargetLogicList;
        newItemId = &newRemoveLogic;
        break;
    case 5:
        targetList = &removeTargetScriptList;
        newItemId = &newRemoveScript;
        break;

    default:
        break;
    }
    std::string nodeName = listName + " List";
    if (ImGui::TreeNode(nodeName.c_str()))
    {
        for (int i = 0; i < targetList->size(); i++)
        {

            ImGui::Text("%d", targetList->at(i));
            ImGui::SameLine();
            std::string btnName("delete ");
            btnName = btnName + std::to_string(targetList->at(i));
            if (ImGui::Button(btnName.c_str()))
            {
                targetList->erase(targetList->begin() + i);
                printf("hello");
            }
        }
        ImGui::InputInt(" ", newItemId);
        std::string createBtnName = std::string("create ") + listName;
        if (ImGui::Button(createBtnName.c_str()))
        {
            targetList->push_back(*newItemId);
            *newItemId = 0;
        }
        ImGui::TreePop();
    }
}
void ScriptEditor::customWidgets()
{
    listTreeNode("Add Signal", 0);
    listTreeNode("Add Logic", 1);
    listTreeNode("Add Script", 2);
    listTreeNode("Remove Signal", 3);
    listTreeNode("Remove Logic", 4);
    listTreeNode("Remove Script", 5);
}
void ScriptEditor::getInfo(Core::Script *s)
{
    scriptId = s->getScriptId();
    Core::ScriptUnion su = s->getScript();
    strcpy_s(scriptName, s->getScriptName().c_str());
    switch (s->getScriptType())
    {
    case Core::ScriptType::Custom:
        addTargetSignalList = su.scriptCustom.getAddTargetSignalList();
        addTargetLogicList = su.scriptCustom.getAddTargetLogicList();
        addTargetScriptList = su.scriptCustom.getAddTargetScriptList();
        removeTargetSignalList = su.scriptCustom.getRemoveTargetSignalList();
        removeTargetLogicList = su.scriptCustom.getRemoveTargetLogicList();
        removeTargetScriptList = su.scriptCustom.getRemoveTargetScriptList();
        break;

    default:
        break;
    }
}