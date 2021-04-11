#include "windows/MapEditor.h"
#include "UndoRedo.h"
#include <string>


void MapEditor::draw() {
	// Map editor
    static char map_name[128] = "";
    static int dim1 = 0;
    static int dim2 = 0;
    static int tileSize = 0;
    bool map_info = false;
    Core::MapPage *map_page = NULL;
    Core::Map *new_map = NULL;
	Core::Game* game = editor->getGamePtr();
    if (visible)
    {
        // possibly implement a new function here for readability purposes

        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        // map editor
        if (ImGui::Begin("Map Editor", &visible))
        {
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
            if (ImGui::Button("Create Map"))
            {
                //creates a new map with map_name specified by user and dimensions as specified by user
                //UNDO
                std::string mname = map_name;
                int savedDim1 = dim1;
                int savedDim2 = dim2;
                auto action = [this, &new_map, &map_page, mname, savedDim1, savedDim2]() {
                    new_map = new Core::Map(mname, glm::vec2(savedDim1, savedDim2), 64);
                    map_page = editor->getGamePtr()->createMapPage(mname, new_map);
                    new_map->setName(mname);
                    new_map->setDimensions(glm::vec2(savedDim1, savedDim2));
                };
                auto restore = [this, mname]() {
                    editor->getGamePtr()->deletePage(mname);
                };
                pushAction(action, restore);
                action();
                //ENDUNDO
                //TODO: render new map
                // SETUP THE MAP CBO IF NEEDED
                editor->setCurrentMap(game->createMapOnDefaultMapPage(map_name, dim2, dim1, tileSize));
                map_name[0] = '\0';
                editor->getWindowList()[MAPVIEW]->setVisible(true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete Map"))
            {
                //creates a map with 0x0 dimensions and an empty name
                /*
                memset(map_name, 0, 128);
                new_map = new Core::Map(map_name, glm::vec2(dim1, dim2), 64);
                map_page = game->createMapPage(map_name, new_map);
                new_map->setName(map_name);
                new_map->setDimensions(glm::vec2(dim1, dim2));
                delete_success = true;
                */
                dim1 = 0;
                dim2 = 0;
                tileSize = 0;
                game->deleteDefaultMapPageCurrentMap();
                editor->setCurrentMap(nullptr);
                editor->showDeleteSuccessPopup();
            }
            if (ImGui::Button("Show Information"))
            {
                map_info = true;
            }

            if (map_info)
            {
                ImGui::OpenPopup("Map Information");
                map_info = false;
            }

            // Map information popup
            if (ImGui::BeginPopup("Map Information"))
            {
                Core::Map* currMap = editor->getCurrentMap();
                ImGui::Text("Map Name:");
                ImGui::SameLine();
                if (currMap != nullptr)
                {
                    ImGui::Text(currMap->getName().c_str());
                }
                else
                {
                    ImGui::Text("no map selected");
                }
                ImGui::Text("Dimensions:");
                ImGui::SameLine();
                glm::ivec2 dims;
                if (currMap != nullptr)
                {
                    dims = currMap->getDimensions();
                }
                else
                {
                    dims = glm::ivec2(0, 0);
                }
                ImGui::Text("%i Columns x %i Rows", dims.x, dims.y);

                if (currMap != nullptr)
                {
                    ImGui::Text("Tile size: %i", currMap->getTileSize());
                }
                else
                {
                    ImGui::Text("Tile size: 0");
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
}