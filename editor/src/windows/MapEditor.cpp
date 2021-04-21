#include "windows/MapEditor.h"
#include "windows/MapWindow.h"
#include "UndoRedo.h"
#include <string>

void MapEditor::draw() {
	// Map editor
	bool map_info = false;
	Core::Game *game = editor->getGamePtr();
	if (visible) {
		// possibly implement a new function here for readability purposes

		// set the windows default size
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		// map editor
		if (ImGui::Begin("Map Editor", &visible)) {
			std::string mapLabel;
			if (editor->getCurrentMap() != nullptr) {
				mapLabel = editor->getCurrentMap()->getName();
			}
			if (ImGui::Button("Create Map")) {
				ImGui::OpenPopup("create_map_popup");
			}
			drawCreateMapPopup();
			ImGui::SameLine();
			if (ImGui::Button("Delete Map")) {
				game->deleteMap(editor->getCurrentMap());
				editor->setCurrentMap(nullptr);
				editor->showDeleteSuccessPopup();
			}
			ImGui::Text("Current Map");
			ImGui::SameLine();
			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("##currentMap", mapLabel.c_str(), 0)) {
				for (auto map : editor->getGamePtr()->getMapList()) {
					const bool isSelected = (map == editor->getCurrentMap());
					if (ImGui::Selectable(map->getName().c_str(), isSelected)) {
						editor->setCurrentMap(map);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			Core::Map *currMap = editor->getCurrentMap();
			if (currMap != nullptr) {
				ImGui::Text("Map Name:");
				ImGui::SameLine();
				if (currMap != nullptr) {
					ImGui::Text(currMap->getName().c_str());
				} else {
					ImGui::Text("no map selected");
				}
				ImGui::Text("Dimensions:");
				ImGui::SameLine();
				glm::ivec2 dims;
				if (currMap != nullptr) {
					dims = currMap->getDimensions();
				} else {
					dims = glm::ivec2(0, 0);
				}
				ImGui::Text("%i Columns x %i Rows", dims.x, dims.y, currMap->getTileSize());
				ImGui::Text("Tile Size: %i", currMap->getTileSize());
				ImGui::Text("");
				if (currMap != nullptr) {
					switch (mode) {
						case EditMode::Collision:
							ImGui::Text("Edit Mode: Collision");
							break;
						case EditMode::Sprite:
							ImGui::Text("Edit Mode: Sprite");
							break;
					}

					if (ImGui::Button("Collision Mode")) {
						mode = EditMode::Collision;
					}

					if (ImGui::Button("Sprite Mode")) {
						mode = EditMode::Sprite;
					}
					ImGui::Text("Sprite ID");
					ImGui::SameLine();
					ImGui::PushItemWidth(100);
					ImGui::InputInt("", &mSelectedSpriteID, 1, 5);
				} else {
					ImGui::Text("Tile size: 0");
				}
			}
		}
		ImGui::End();
	}
}

void MapEditor::drawCreateMapPopup() {
	static char map_name[128] = "";
	static int dim1 = 0;
	static int dim2 = 0;
	static int tileSize = 0;
	if (ImGui::BeginPopup("create_map_popup")) {
		ImGui::Text("Enter Map Name:");
		ImGui::PushItemWidth(200);
		ImGui::InputText(" ", map_name, IM_ARRAYSIZE(map_name));
		ImGui::Text("Rows:    ");
		ImGui::PushItemWidth(100);
		ImGui::SameLine();
		ImGui::SliderInt("##1", &dim1, 0, 50);
		ImGui::Text("Columns: ");
		ImGui::SameLine();
		ImGui::SliderInt("##2", &dim2, 0, 50);
		ImGui::Text("Tile size: ");
		ImGui::SameLine();
		ImGui::SliderInt("##3", &tileSize, 0, 128);

		if (ImGui::Button("Create")) {
			// creates a new map with map_name specified by user and
			// dimensions as specified by user UNDO
			std::string mname = map_name;
			glm::ivec2 dimensions = glm::ivec2(dim1, dim2);
			int savedTileSize = tileSize;
			auto action = [this, mname, dimensions, savedTileSize]() {
				Core::Map *newMap = new Core::Map(mname, dimensions, savedTileSize);
				newMap->setName(mname);
				newMap->setDimensions(dimensions);
				editor->setCurrentMap(newMap);
				int mapIndex = editor->getGamePtr()->addMap(newMap);
			};
			auto restore = [this, mname]() {
				editor->getGamePtr()->deleteMap(mname);
			};
			pushAction(action, restore);
			action();
			// ENDUNDO
			// SETUP THE MAP CBO IF NEEDED
			map_name[0] = '\0';
			dim1 = 0;
			dim2 = 0;
			tileSize = 0;
			editor->getWindowList()[MAPVIEW]->setVisible(true);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
