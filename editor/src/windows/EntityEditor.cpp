#include "windows/EntityEditor.h"
#include "UndoRedo.h"


static void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void EntityEditor::draw()
{
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        entityInfo = false;
        static bool collideable = false;
        if (ImGui::Begin("Entity Editor", &visible))
        {
            Core::Page* currPage = editor->getGamePtr()->getCurrPage();
            ImGui::PushItemWidth(200);
            ImGui::Text("Enter Entity Name:");
            ImGui::InputText("##entity_name", entityName, IM_ARRAYSIZE(entityName));

            ImGui::Checkbox("Collideable?", &collideable);
            if (collideable)
            {
                //TODO: call collision functions
            }

            if (ImGui::Button("Create New Entity"))
            {
                if (strlen(entityName) != 0)
                {
                    std::string e = entityName;
                    auto action = [this, e]() {
                        editor->getGamePtr()->getCurrPage()->createEntity(e);
                    };
                    auto restore = [this, e]() {
                        editor->getGamePtr()->getCurrPage()->deleteEntity(e);
                    };
                    pushAction(action, restore);
                    action();
                    //ENDUNDO
                    // clear the buffer after use
                    entityName[0] = '\0';
                }
            }
            
            ImGui::Text("");
            ImGui::Text("Current Entity: %s", editor->getCurrentComponentList()[CUR_ENTITY].c_str());
            static int current_entity = 0;
            if (ImGui::BeginListBox("", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                // Set default selected entity to be the first in the entity list
                if (editor->getCurrentComponentList()[CUR_ENTITY] == "No Component Selected" && currPage->getEntityList().size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_ENTITY] = currPage->getEntityList()[0]->getName();
                }

                for (int i = 0; i < currPage->getEntityList().size(); i++)
                {
                    auto entity = currPage->getEntityList()[i];
                    bool is_selected = (current_entity == i);
                    if (ImGui::Selectable(entity->getName().c_str(), is_selected))
                    {
                        current_entity = i; 
                        x_pos = entity->getLocation().x;
                        y_pos = entity->getLocation().y;
                        editor->getCurrentComponentList()[CUR_ENTITY] = entity->getName();
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndListBox();
            }

            if (ImGui::Button("Change Name"))
            {
                if (editor->getCurrentComponentList()[CUR_ENTITY] != "No Component Selected")
                {
                    for (Core::Entity *e : currPage->getEntityList())
                    {
                        if (e->getName() == editor->getCurrentComponentList()[CUR_ENTITY])
                        {
                            e->setName(entityName);
                            editor->getCurrentComponentList()[CUR_ENTITY] = entityName;
                            break;
                        }
                    }
                }

                // clear the buffer after use
                entityName[0] = '\0';
            }
            ImGui::SameLine();

            std::string currentEntityName = editor->getCurrentComponentList()[CUR_ENTITY];
            if (ImGui::Button("Delete Entity") && currentEntityName != "No Component Selected")
            {
                int idx = -1;
                size_t original = editor->getGamePtr()->getCurrPage()->getEntityList().size();
                bool isCtrlEntity = false;
                auto& eList = currPage->getEntityList();
                for (int i = 0; i < eList.size(); i++)
                {
                    if (eList[i]->getName() == currentEntityName)
                    {
                        idx = i;
                        if (eList[i] == currPage->getCtrlEntity())
                        {
                            isCtrlEntity = true;
                        }
                        break;
                    }
                }
                if (idx > -1)
                {
                    Core::Entity savedEntity = *eList[idx];
                    auto action = [this, currentEntityName]() {
                        editor->getGamePtr()->getCurrPage()->deleteEntity(currentEntityName);
                    };
                    auto restore = [this, idx, savedEntity, isCtrlEntity]() {
                        Core::Entity* newEntity = new Core::Entity(savedEntity);
                        editor->getGamePtr()->getCurrPage()->getEntityList()
                            .insert(editor->getGamePtr()->getCurrPage()->getEntityList().begin() + idx, newEntity);
                        if (isCtrlEntity)
                        {
                            editor->getGamePtr()->getCurrPage()->setCtrlEntity(newEntity);
                        }
                    };
                    // Only delete if the entity was found
                
                    action();
                    editor->getCurrentComponentList()[CUR_ENTITY] = idx < currPage->getEntityList().size() ?
                        currPage->getEntityList()[idx]->getName() : currPage->getEntityList()[idx - 1]->getName();
                        
                    if (currPage->getEntityList().size() < original)
                    {
                        editor->getCurrentComponentList()[CUR_ENTITY] = eList[0]->getName();
                        editor->showDeleteSuccessPopup();
                        pushAction(action, restore); // UNDO
                        // memset to clear the buffer after use
                        entityName[0] = '\0';
                    }
                }
            }
            if (ImGui::Button("Show Information"))
            {
                entityInfo = true;
            }

            //get x pos and y pos for entity's new location
            if (editor->getCurrentComponentList()[CUR_ENTITY] == "No Component Selected")
            {
                x_pos = 0;
                y_pos = 0;
            }

            Core::Entity *moved_ent = NULL;
            ImGui::Text("");
            ImGui::Text("X Pos:");
            ImGui::PushItemWidth(80);
            ImGui::SameLine();
            ImGui::InputInt("##x_pos", &x_pos);
            ImGui::SameLine();
            HelpMarker("Choose a value between [0,1250]");
            ImGui::Text("Y Pos:");
            ImGui::SameLine();
            ImGui::InputInt("##y_pos", &y_pos);
            ImGui::SameLine();
            HelpMarker("Choose a value between [0,700]");
            if (ImGui::Button("Change Entity Position"))
            {
                //these are totally arbitrary #s to try to ensure entities cant be rendered outside the game view window when at max size
                //should probably change to use monitor display settings later on
                if (x_pos >= 0 && x_pos <= 1250 && y_pos >= 0 && y_pos <= 700)
                {
                    for (Core::Entity *e : currPage->getEntityList())
                    {
                        if (e->getName() == editor->getCurrentComponentList()[CUR_ENTITY])
                        {
                            //set location as specified by user
                            //UNDO
                            auto pos = glm::vec2(x_pos, y_pos);
                            auto currentPos = e->getLocation();
                            auto action = [e, pos, &moved_ent]() {
                                e->setLocation(pos);
                                moved_ent = e;
                            };
                            auto restore = [e, currentPos, &moved_ent]() {
                                e->setLocation(currentPos);
                                moved_ent = e;
                            };
                            pushAction(action, restore);
                            action();
                            //END UNDO

                            break;
                        }
                    }
                    //render the new entity location
                    if (moved_ent != NULL)
                    {
                        moved_ent->render();
                    }
                }
                else
                {
                    x_pos = 0;
                    y_pos = 0;
                }
            }
        }

        if (entityInfo)
        {
            ImGui::OpenPopup("Entity Information");
            entityInfo = false;
        }

        // Entity information popup
        if (ImGui::BeginPopup("Entity Information"))
        {
            ImGui::Text("Entity Name: %s", editor->getCurrentComponentList()[CUR_ENTITY].c_str());
            ImGui::Text("Entity Position: %d, %d", x_pos, y_pos);
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}
