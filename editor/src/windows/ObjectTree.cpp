#include "windows/ObjectTree.h"


void ObjectTree::draw()
{
    if (visible)
    {
        Core::Game *game = editor->getGamePtr();

        ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
        ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);
        if (ImGui::Begin("Object Tree", &visible))
        {
#ifdef __TEST_EDITOR
            treeclicked = true;
#endif
            //entities node
            if (ImGui::TreeNodeEx("Entities", base_flags, "Entities")
#ifdef __TEST_EDITOR
                || testbool
#endif
            )

            {
                if (game != nullptr)
                {
                    static int selected = -1;
                    int node_clicked = -1;
                    int index = 0;
                    std::vector<Core::Entity *> elist = editor->getCurrentPage()->getEntityList();
                    for (Core::Entity *e : elist)
                    {
                        ImGuiTreeNodeFlags node_flags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                        if (index == selected)
                            node_flags |= ImGuiTreeNodeFlags_Selected;
                        ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, e->getName().c_str());
                        if (ImGui::IsItemClicked())
                        {
                            node_clicked = index;
                        }
                        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
                        {
                            editor->getWindowList()[ENTITYEDITOR]->setVisible(true);
                            editor->getCurrentComponentList()[CUR_ENTITY] = e->getName();
                        }
                        index++;
                    }
                    if (node_clicked != -1)
                    {
                        selected = node_clicked;
                    }
                }
                ImGui::TreePop();
            }
            //logic node
            /*if (ImGui::TreeNodeEx("Logic", base_flags, "Logic"))
            {
                ImGui::TreePop();
            }*/
            //pages node
            if (ImGui::TreeNodeEx("Pages", base_flags, "Pages"))
            {
                if (game != nullptr)
                {
                    static int selected = -1;
                    int node_clicked = -1;
                    int index = 0;
                    std::vector<Core::Page *> plist = *game->getPageList();
                    for (int i = 0; i < plist.size(); i++)
                    {
                        Core::Page *p = plist[i];

                        ImGuiTreeNodeFlags node_flags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                        if (index == selected)
                            node_flags |= ImGuiTreeNodeFlags_Selected;
                        ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, p->getName().c_str());
                        if (ImGui::IsItemClicked())
                        {
                            node_clicked = index;
                        }
                        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
                        {
                            editor->getWindowList()[PAGEEDITOR]->setVisible(true);
                            editor->getCurrentComponentList()[CUR_PAGE] = p->getName();
                        }
                        index++;
                    }
                    if (node_clicked != -1)
                    {
                        selected = node_clicked;
                    }
                }
                ImGui::TreePop();
            }
            /*if (ImGui::TreeNodeEx("Scripts", base_flags, "Scripts"))
            {
                ImGui::TreePop();
            }*/
            if (ImGui::TreeNodeEx("Sprites", base_flags, "Sprites"))
            {
                if (game != nullptr)
                {
                    static int selected = -1;
                    int node_clicked = -1;
                    int index = 0;
                    for (auto &[key, value] : game->getSprites())
                    {
                        ImGuiTreeNodeFlags node_flags = base_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                        if (index == selected)
                            node_flags |= ImGuiTreeNodeFlags_Selected;
                        ImGui::TreeNodeEx((void *)(intptr_t)index, node_flags, value->getName().c_str());
                        if (ImGui::IsItemClicked())
                        {
                            node_clicked = index;
                        }
                        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
                        {
                            editor->getWindowList()[SPRITEEDITOR]->setVisible(true);
                            editor->getCurrentComponentList()[CUR_SCRIPT] = value->getName();
                        }
                        index++;
                    }
                    if (node_clicked != -1)
                    {
                        selected = node_clicked;
                    }
                }
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}