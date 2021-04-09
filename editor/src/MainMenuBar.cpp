#include "MainMenuBar.h"
#include "UndoRedo.h"


void MainMenuBar::draw() {
	if (ImGui::BeginMainMenuBar()) {
		// File menu
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Project")) {
				editor->createGame();
			}
			if (ImGui::MenuItem("Open Project")) {
				openDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_NoTitleBar);
				openDialog.SetTypeFilters({".gdata"});
				openDialog.Open();
			}
			if (ImGui::MenuItem("Delete Project")) {
				ImGui::OpenPopup("Delete Successful");
        		ImGui::OpenPopup("Delete Project");
			}
			/*if ( ImGui::MenuItem ( "Export Project" ) )
			{
				// call export function from VM team
				// not yet implemented as of sprint 1
			}*/

			// Can't save empty game
			// save button can be grey in the future ( no need to implement)
			if (editor->getGamePtr() != nullptr) {
				if (ImGui::MenuItem("Save")) {
					if (editor->getGameFilePath().empty()) {
						editor->saveGame();
					} else {
						ImGui::OpenPopup("Save As");
					}
				}
				if (ImGui::MenuItem("Save As")) {
					ImGui::OpenPopup("Save As");
				}
			}
			ImGui::EndMenu();
		}

		// only display the edit and view menus if a project exists
		if (editor->getGamePtr() != nullptr) {
			// Edit menu
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
					undo();
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
					redo();
				}
				ImGui::EndMenu();
			}

			// view menu
			if (ImGui::BeginMenu("View")) {
				auto& windowList = editor->getWindowList();
				ImGui::MenuItem("Object Tree", "", windowList[OBJECTTREE]->getVisiblePtr());
				ImGui::MenuItem("Game View", "", windowList[GAMEVIEW]->getVisiblePtr());
				ImGui::MenuItem("Entity Editor", "", windowList[ENTITYEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Page Editor", "", windowList[PAGEEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Map Editor", "", windowList[MAPEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Script Editor", "", windowList[SCRIPTEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Sprite Editor", "", windowList[SPRITEEDITOR]->getVisiblePtr());
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}

	drawOpenDialog();
	drawSaveAsPopup();
	drawDeleteProjectPopup();	
}

void MainMenuBar::drawOpenDialog() {
	// Open dialog selection return
    openDialog.Display();
    if (openDialog.HasSelected())
    {
		editor->loadGame(openDialog.GetSelected().string());
        openDialog.ClearSelected();
    }
}

void MainMenuBar::drawDeleteProjectPopup() {
	if (ImGui::BeginPopup("Delete Project"))
    {
        ImGui::Text("Are you sure you want to delete a project? Click outside of this popup to cancel.");
        if (ImGui::Button("Yes"))
        {
            delDialog = ImGui::FileBrowser(
                ImGuiFileBrowserFlags_NoTitleBar);
            delDialog.Open();
        }
        ImGui::EndPopup();
    }

	// Delete dialog selection return
    delDialog.Display();
    if (delDialog.HasSelected())
    {
        DeleteFile(delDialog.GetSelected().string().c_str());
        delDialog.ClearSelected();
        ImGui::OpenPopup("Deleted Successfully");
    }
}

void MainMenuBar::drawSaveAsPopup() {
	static char name[128] = "";
    if (ImGui::BeginPopup("Save As"))
    {
        ImGui::Text("Enter the name of your project.");
        ImGui::InputText("", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("Save"))
        {
            // init file browser ( update info every time it was opened)
            saveDialog = ImGui::FileBrowser(
                ImGuiFileBrowserFlags_NoTitleBar |
                ImGuiFileBrowserFlags_SelectDirectory |
                ImGuiFileBrowserFlags_CreateNewDir);
            saveDialog.Open();

            if (!saveDialog.IsOpened())
            {
                editor->getGamePtr()->setGameName(name);
				name[0] = '\0';
            }
        }
        ImGui::EndPopup();
    }

	saveDialog.Display();
    if (saveDialog.HasSelected())
    {
		editor->saveGameAs(saveDialog.GetSelected().string().append("//").append(name).append(".gdata"));
        // nlohmann::json *content = game->serialize();
        // WriteFile(dir, (content->dump(2)));
        // // pointer deletion
        // delete (content);
        // isSaved = true;
        saveDialog.ClearSelected();
        ImGui::OpenPopup("Saved Successfully");
    }
}
