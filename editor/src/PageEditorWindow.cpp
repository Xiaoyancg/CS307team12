#include "PageEditorWindow.h"
#include "Editor.h"


void PageEditorWindow::draw() {
	if (visible)
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char page_name[128] = "";
        static char chosen_type[128] = "";
        bool page_info = false;
		Core::Game* game = editor->getGamePtr();
        if (ImGui::Begin("Page Editor", &visible))
        {
            ImGui::Text("Enter Page Name:");
            ImGui::PushItemWidth(200);
            ImGui::InputText(" ", page_name, IM_ARRAYSIZE(page_name));
            const char *page_options[] = {"Page", "Menu"};
            static int current_item = 0;
            ImGui::Text("Select Page Type:");
            if (ImGui::BeginListBox("", ImVec2(200, 2 * ImGui::GetTextLineHeightWithSpacing())))
            {
                for (int n = 0; n < IM_ARRAYSIZE(page_options); n++)
                {
                    const bool is_selected = (current_item == n);
                    if (ImGui::Selectable(page_options[n], is_selected))
                        current_item = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Create New Page"))
            {
                if (strlen(page_name) != 0)
                {
                    if (strcmp(page_options[current_item], "Menu") == 0)
                    {
                        //UNDO
                        std::string pname = page_name;
                        auto action = [this, pname]() {
                            editor->getGamePtr()->createMenuPage(pname);
                        };
                        auto restore = [this, pname]() {
                            editor->getGamePtr()->deletePage(pname);
                        };
                        pushAction(action, restore);
                        action();
                        //ENDUNDO
                        // memset to clear the buffer after use
                        memset(page_name, 0, 128);
                    }
                    else
                    {
                        //UNDO
                        std::string pname = page_name;
                        auto action = [this, pname]() {
                            editor->getGamePtr()->createPage(pname);
                        };
                        auto restore = [this, pname]() {
                            editor->getGamePtr()->deletePage(pname);
                        };
                        pushAction(action, restore);
                        action();
                        //ENDUNDO
                        // memset to clear the buffer after use
                        memset(page_name, 0, 128);
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete This Page"))
            {
                size_t original = game->getPageList()->size();
                //UNDO
                std::string pname = page_name;
                int idx = 0;
                auto &pList = *game->getPageList();
                for (int i = 0; i < pList.size(); i++)
                {
                    if (pList[i]->getName() == page_name)
                    {
                        idx = i;
                        break;
                    }
                }
                Core::Page savedPage = *pList[idx];
                auto action = [this, pname]() {
                    editor->getGamePtr()->deletePage(pname);
                };
                auto restore = [this, idx, savedPage]() {
                    Core::Page *newPage = new Core::Page(savedPage);
                    editor->getGamePtr()->getPageList()->insert(editor->getGamePtr()->getPageList()->begin() + idx, newPage);
                };
                action();
                //ENDUNDO
                if (game->getPageList()->size() < original)
                {
                    editor->markDeleteSuccess();
                    pushAction(action, restore); // UNDO
                    // memset to clear the buffer after use
                    memset(page_name, 0, 128);
                }
            }
            if (ImGui::Button("Show Page Information"))
            {
                page_info = true;
            }
        }

        if (page_info)
        {
            ImGui::OpenPopup("Page Information");
            page_info = false;
        }

        // Page information popup
        if (ImGui::BeginPopup("Page Information"))
        {
            ImGui::Text("Page Name:");
            ImGui::SameLine();
            ImGui::Text(editor->getCurrentPage()->getName().c_str());
            std::vector<Core::Page *> plist = *game->getPageList();
            ImGui::Text("");
            ImGui::Text("Page List: ");
            for (Core::Page *p : plist)
            {
                ImGui::Text(p->getName().c_str());
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}