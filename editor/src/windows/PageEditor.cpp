#include "windows/PageEditor.h"
#include "UndoRedo.h"


void PageEditor::draw() {
	if (visible)
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        static char page_name[128] = "";
        static char chosen_type[128] = "";
        static int current_item = 0;
        static int current_page = 0;
        const char *page_options[] = {"Page", "Menu"};
        bool page_info = false;
		Core::Game* game = editor->getGamePtr();
        if (ImGui::Begin("Page Editor", &visible))
        {
            ImGui::PushItemWidth(200);
            ImGui::Text("Input Page Name & Type:");
            ImGui::InputText(" ", page_name, IM_ARRAYSIZE(page_name));
            if (ImGui::BeginListBox("##1", ImVec2(200, 2 * ImGui::GetTextLineHeightWithSpacing())))
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

            if (ImGui::Button("Create Page"))
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

            ImGui::Text("");
            ImGui::Text("Current Page: %s", editor->getCurrentComponentList()[CUR_PAGE].c_str());
            if (ImGui::BeginListBox("##2", ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
            {
                auto& pList = game->getPageList();
                // Set default selected page to be the first in the page list
                if (editor->getCurrentComponentList()[CUR_PAGE] == "No Component Selected" && game->getPageList().size() > 0)
                {
                    editor->getCurrentComponentList()[CUR_PAGE] = pList[0]->getName();
                }
                for (int i = 0; i < game->getPageList().size(); i++)
                {
                    auto page = game->getPageList()[i];
                    const bool is_selected = (current_page == i);
                    if (ImGui::Selectable(page->getName().c_str(), is_selected))
                    {
                        current_page = i;
                        editor->getCurrentComponentList()[CUR_PAGE] = page->getName();
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
                if (editor->getCurrentComponentList()[CUR_PAGE] != "No Component Selected")
                {
                    auto& pList = game->getPageList();
                    for (int i = 0; i < pList.size(); i++)
                    {
                        if (pList[i]->getName() == editor->getCurrentComponentList()[CUR_PAGE])
                        {
                            pList[i]->setName(page_name);
                            editor->getCurrentComponentList()[CUR_PAGE] = page_name;
                        }
                    }
                }

                // memset to clear the buffer after use
                memset(page_name, 0, 128);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Page") && editor->getCurrentComponentList()[CUR_PAGE] != "No Component Selected")
            {
                size_t original = game->getPageList().size();
                std::string pname = editor->getCurrentComponentList()[CUR_PAGE];
                int idx = 0;
                auto &pList = game->getPageList();
                for (int i = 0; i < pList.size(); i++)
                {
                    if (pList[i]->getName() == editor->getCurrentComponentList()[CUR_PAGE])
                    {
                        idx = i;
                        break;
                    }
                }
                if (idx != -1 && game->getPageList().size() != 1)
                {
                    Core::Page savedPage = *pList[idx];
                    auto action = [this, pname]() {
                        editor->getGamePtr()->deletePage(pname);
                    };
                    auto restore = [this, idx, savedPage]() {
                        Core::Page *newPage = new Core::Page(savedPage);
                        editor->getGamePtr()->getPageList().insert(editor->getGamePtr()->getPageList().begin() + idx, newPage);
                    };
                    pushAction(action, restore);
                    action();
                    editor->showDeleteSuccessPopup();
                    editor->getCurrentComponentList()[CUR_PAGE] = pList[0]->getName();
                }
            }
            if (ImGui::Button("Show Information"))
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
            auto& pList = game->getPageList();
            ImGui::Text("Page Name: %s", editor->getCurrentComponentList()[CUR_PAGE].c_str());
            for (int i = 0; i < pList.size(); i++)
            {
                if (pList[i]->getName() == editor->getCurrentComponentList()[CUR_PAGE])
                {
                    if (!strcmp(typeid(*pList[i]).name(), "class Core::MenuPage"))
                    {
                        ImGui::Text("Page Type: Menu");
                    }             
                    else
                    {
                        ImGui::Text("Page Type: Page");
                    }
                }
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}