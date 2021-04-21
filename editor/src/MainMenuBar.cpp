#include "MainMenuBar.h"
#include "UndoRedo.h"


void MainMenuBar::draw() {
	auto& windowList = editor->getWindowList();
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
        		deleteProjectPopup = true;
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
					if (!editor->getGameFilePath().empty()) {
						editor->saveGame();
						editor->showSaveSuccessPopup();
					} else {
						saveAsPopup = true;
					}
				}
				if (ImGui::MenuItem("Save As")) {
					saveAsPopup = true;
				}
			}
			ImGui::MenuItem("Editor Theme", "", windowList[STYLEEDITOR]->getVisiblePtr());
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
				ImGui::MenuItem("Game View", "", windowList[GAMEVIEW]->getVisiblePtr());
				ImGui::MenuItem("Map View", "", windowList[MAPVIEW]->getVisiblePtr());
				ImGui::MenuItem("Object Tree", "", windowList[OBJECTTREE]->getVisiblePtr());
				ImGui::EndMenu();
			}

			//add editor
			if (ImGui::BeginMenu("Open Editor")) {
				ImGui::MenuItem("Map Editor", "", windowList[MAPEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Entity Editor", "", windowList[ENTITYEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Sprite Editor", "", windowList[SPRITEEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Page Editor", "", windowList[PAGEEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Script Editor", "", windowList[SCRIPTEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Signal Editor", "", windowList[SIGNALEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Status Editor", "", windowList[STATUSEDITOR]->getVisiblePtr());
				ImGui::MenuItem("Logic Editor", "", windowList[LOGICEDITOR]->getVisiblePtr());
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
	// Initial deletion prompt
	if (deleteProjectPopup) {
		ImGui::OpenPopup("delete_project_popup");
		deleteProjectPopup = false;
	}	
	if (ImGui::BeginPopup("delete_project_popup"))
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
        ImGui::OpenPopup("project_deleted_popup");
    }

	// Successful Project deletion popup
    if (projectDeletedPopup) {
        ImGui::OpenPopup("project_deleted_popup");
        projectDeletedPopup = false;
    }
    if (ImGui::BeginPopup("project_deleted_popup"))
    {
        ImGui::Text("Project deleted successfully!");
        ImGui::EndPopup();
    }
}

void MainMenuBar::drawSaveAsPopup() {
	static char name[128] = "";
	if (saveAsPopup) {
		ImGui::OpenPopup("save_as_popup");
		saveAsPopup = false;
	}
    if (ImGui::BeginPopup("save_as_popup"))
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
        }
        ImGui::EndPopup();
    }

	saveDialog.Display();
    if (saveDialog.HasSelected())
    {
        editor->getGamePtr()->setGameName(name);
		editor->saveGameAs(saveDialog.GetSelected().string().append("//").append(name).append(".gdata"));
		name[0] = '\0';
        saveDialog.ClearSelected();
        editor->showSaveSuccessPopup();
    }
}
