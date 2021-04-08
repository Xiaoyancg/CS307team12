#include "MainMenuBar.h"

void MainMenuBar::draw() {
	if (ImGui::BeginMainMenuBar()) {
		// File menu
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Project")) {
				editor->createTexCBO();
				editor->createMapTexCBO();
				game = new Core::Game(editor->getTexCBO(), editor->getMapTexCBO());
				currPage = game->getCurrPage();
				game->initShader();
				windowList[GAMEVIEW]->setVisible(true);
				currentComponent[CUR_ENTITY] = "No Component Selected";
				// When user new project, it won't save
				// User should call save manually
				// selection[SAVEAS] = true;
			}
			if (ImGui::MenuItem("Open Project")) {
				openDialog =
					ImGui::FileBrowser(ImGuiFileBrowserFlags_NoTitleBar);
				openDialog.SetTypeFilters({".gdata"});
				openDialog.Open();
			}
			if (ImGui::MenuItem("Delete Project")) {
				selection[DELETEPROJECT] = true;
			}
			/*if ( ImGui::MenuItem ( "Export Project" ) )
			{
				// call export function from VM team
				// not yet implemented as of sprint 1
			}*/

			// Can't save empty game
			// save button can be grey in the future ( no need to implement)
			if (game != nullptr) {
				if (ImGui::MenuItem("Save")) {
					if (isSaved) {
						nlohmann::json *content = game->serialize();
						WriteFile(dir, (content->dump(2)));
						// pointer deletion
						delete (content);
						selection[SAVEPOPUP] = true;
					} else {
						selection[SAVEAS] = true;
					}
				}
				if (ImGui::MenuItem("Save As")) {
					selection[SAVEAS] = true;
				}
			}
			ImGui::EndMenu();
		}

		// only display the edit and view menus if a project exists
		if (game != nullptr) {
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
}