#include "windows/SpriteEditor.h"


void SpriteEditor::draw() {
	// Sprite editor
    if (visible)
    {
        // set the windows default size
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        // NOTE: Sprite name and ID do NOT have to be set to import a sprite. By default, the editor will use the filename if no name is given,
        //       and will find and return the next usable ID if none is explicitly requested on Sprite creation.
        static char sprite_name[128] = "";
        static char spriteIDInput[128] = "";
        int spriteID = -1; // -1 means the ID has not been sent (can't default to 0 because 0 is a valid ID)
        bool sprite_info = false;
        if (ImGui::Begin("Sprite Editor", &visible))
        {

            /////////////////////////////////
            ImGui::PushItemWidth(200);
            ImGui::Text("Enter Sprite Name:");
            ImGui::InputText("##sprite_name", sprite_name, IM_ARRAYSIZE(sprite_name));

            ImGui::PushItemWidth(200);

            ImGui::Text("Set Sprite ID:");
            ImGui::InputText("##sprite_id", spriteIDInput, IM_ARRAYSIZE(spriteIDInput), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::Text(editor->getCurrentComponentList()[CUR_SPRITE].c_str());
            if (ImGui::Button("Import Sprite"))
            {
                importDialog = ImGui::FileBrowser(
                    ImGuiFileBrowserFlags_NoTitleBar);
                importDialog.SetTypeFilters({".jpg", ".png"});
                importDialog.Open();
            }

            if (ImGui::Button("Show Information"))
            {
                sprite_info = true;
            }
            if (ImGui::Button("Delete Sprite"))
            {
                // TODO: Implement sprite deletion
            }
        }

        // Check if the input is null (using != "" in this situation causes issues)
        if (spriteIDInput[0] != 0)
        {
            spriteID = atoi(spriteIDInput);
        }

        if (sprite_info)
        {
            ImGui::OpenPopup("Sprite Information");
            sprite_info = false;
        }

        // Sprite information popup
        if (ImGui::BeginPopup("Sprite Information"))
        {
            // Sprite name, dimensions, ID?
            if (editor->getCurrentComponentList()[CUR_SPRITE] == "No Component Selected")
            {
                ImGui::Text("Current Sprite Name: None");
            }
            else
            {
                // Show all sprites
                for (auto &[key, value] : editor->getGamePtr()->getSprites())
                {
                    // Any sprite referenced in the .gdata file will exist in game->getSprites, but may not have been loaded into memory yet.
                    // This is just preventing referencing a null pointer. Once the sprite with the correct ID is loaded, this should correctly show its info
                    if (value)
                    {
                        // Show the current sprite name
                        std::string sprite_info = std::to_string(key).append(": ").append(value->getName());
                        ImGui::Text(sprite_info.c_str());
                    }
                }
            }
            ImGui::EndPopup();
        }

        // Sprite import dialog
        // NOTE: I had to move this here from the bottom section because this needs access to sprite_name
        importDialog.Display();
        if (importDialog.HasSelected())
        {
            // extract just the file name from the selected path
            std::string fileName = importDialog.GetSelected().string().substr(importDialog.GetSelected().string().find_last_of('\\', std::string::npos) + 1, std::string::npos);
            std::string filePath = std::filesystem::relative(importDialog.GetSelected()).string();

            if (sprite_name[0] != 0)
            {
                editor->getCurrentComponentList()[CUR_SPRITE] = sprite_name;
            }
            else
            {
                editor->getCurrentComponentList()[CUR_SPRITE] = fileName;
            }

            if (spriteID >= 0)
            {
                editor->getGamePtr()->createSprite(editor->getCurrentComponentList()[CUR_SPRITE], filePath, spriteID);
            }
            else
            {
                editor->getGamePtr()->createSprite(editor->getCurrentComponentList()[CUR_SPRITE], filePath);
            }

            importDialog.ClearSelected();
            memset(sprite_name, 0, 128);
            memset(spriteIDInput, 0, 128);
        }

        ImGui::End();
    }
}
