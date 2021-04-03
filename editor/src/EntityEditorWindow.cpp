#include "EntityEditorWindow.h"
#include "Editor.h"


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

void EntityEditorWindow::draw()
{
    if (visible)
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        entityName[0] = '\0';
        entityInfo = false;
        if (ImGui::Begin("Entity Editor", &visible))
        {
            Core::Page* currPage = editor->getCurrentPage();
            ImGui::PushItemWidth(200);
            ImGui::Text("Enter Entity Name:");
            ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 260);
            ImGui::Text("Current Entity: %s", editor->getCurrentComponentList()[CUR_ENTITY].c_str());
            // vvv POSSIBLE BUFFER OVERFLOW, CHECK LATER -jack vvv
            ImGui::InputText(" ", entityName, sizeof(entityName));

            if (ImGui::Button("Create New Entity"))
            {
                if (strlen(entityName) != 0)
                {
                    //UNDO
                    Core::Page *p = editor->getCurrentPage();
                    std::string e = entityName;
                    auto action = [p, e]() {
                        p->createEntity(e);
                    };
                    auto restore = [p, e]() {
                        p->deleteEntity(e);
                    };
                    pushAction(action, restore);
                    action();
                    //ENDUNDO
                    // clear the buffer after use
                    entityName[0] = '\0';
                }
            }
            ImGui::SameLine();
            std::string& currentEntityName = editor->getCurrentComponentList()[CUR_ENTITY];
            if (ImGui::Button("Delete This Entity") && currentEntityName != "No Component Selected")
            {
                memcpy(entityName, currentEntityName.c_str(), currentEntityName.size() + 1);
                printf("Deleting entity: %s\n", entityName);
                size_t original = editor->getGamePtr()->getCurrPage()->getEntityList().size();
                //UNDO
                Core::Page *p = currPage;
                std::string e = entityName;
                int idx = -1;
                bool isCtrlEntity = false;
                auto &eList = currPage->getEntityList();
                for (int i = 0; i < eList.size(); i++)
                {
                    if (eList[i]->getName() == entityName)
                    {
                        idx = i;
                        if (eList[i] == currPage->getCtrlEntity())
                        {
                            isCtrlEntity = true;
                        }
                        break;
                    }
                }
                Core::Entity savedEntity = *eList[idx];
                auto action = [p, e]() {
                    p->deleteEntity(e);
                };
                auto restore = [idx, p, savedEntity, isCtrlEntity]() {
                    Core::Entity *newEntity = new Core::Entity(savedEntity);
                    p->getEntityList().insert(p->getEntityList().begin() + idx, newEntity);
                    if (isCtrlEntity)
                    {
                        p->setCtrlEntity(newEntity);
                    }
                };

                // Only delete if the entity was found
                if (idx > -1)
                {
                    action();
                    currentEntityName = idx < currPage->getEntityList().size() ? currPage->getEntityList()[idx]->getName() : currPage->getEntityList()[idx - 1]->getName();
                }
                //ENDUNDO

                if (currPage->getEntityList().size() < original)
                {
                    editor->markDeleteSuccess();
                    pushAction(action, restore); // UNDO
                    // clear the buffer after use
                    entityName[0] = '\0';
                }
            }
            ImGui::Text("");
            ImGui::Text("Select Entity:");
            char **entities_list = (char **)malloc(sizeof(char *) * currPage->getEntityList().size());
            static int current_entity = 0;
            if (ImGui::BeginListBox("", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                // Set default selected entity to be the first in the entity list
                if (editor->getCurrentComponentList()[CUR_ENTITY] == "No Component Selected" && currPage->getEntityList().size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_ENTITY] = currPage->getEntityList()[0]->getName();
                }

                for (int n = 0; n < currPage->getEntityList().size(); n++)
                {
                    std::string ent_name = currPage->getEntityList()[n]->getName().c_str();
                    entities_list[n] = (char *)malloc(ent_name.length() + 1);
                    strncpy(entities_list[n], (char *)currPage->getEntityList()[n]->getName().c_str(), ent_name.length());
                    entities_list[n][ent_name.length()] = '\0';

                    const bool is_selected = (current_entity == n);
                    if (ImGui::Selectable(entities_list[n], is_selected))
                    {
                        current_entity = n;
                        editor->getCurrentComponentList()[CUR_ENTITY] = ent_name;
                        printf("%s\n", editor->getCurrentComponentList()[CUR_ENTITY].c_str());
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();

                    free(entities_list[n]);
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
                        }
                    }
                }

                // clear the buffer after use
                entityName[0] = '\0';
            }
            ImGui::SameLine();
            if (ImGui::Button("Show Entity Information"))
            {
                entityInfo = true;
            }

            //get x pos and y pos for entity's new location
            Core::Entity *moved_ent = NULL;
            ImGui::Text("");
            ImGui::Text("X Pos:");
            ImGui::PushItemWidth(80);
            ImGui::SameLine();
            ImGui::InputInt("##1", &x_pos);
            ImGui::SameLine();
            HelpMarker("Choose a value between [0,1250]");
            ImGui::Text("Y Pos:");
            ImGui::SameLine();
            ImGui::InputInt("##2", &y_pos);
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
            free(entities_list);
        }

        if (entityInfo)
        {
            ImGui::OpenPopup("Entity Information");
            entityInfo = false;
        }

        // Entity information popup
        if (ImGui::BeginPopup("Entity Information"))
        {
            std::vector<Core::Entity *> elist = editor->getCurrentPage()->getEntityList();
            ImGui::Text("Entity List: ");
            for (Core::Entity *e : elist)
            {
                ImGui::Text(e->getName().c_str());
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}
