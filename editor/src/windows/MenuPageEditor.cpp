#include "windows/MenuPageEditor.h"

void MenuPageEditor::draw() {
	static char sprite_name[128] = "";
	static char spriteIDInput[128] = "";
	static int loc_x = 0;
	static int loc_y = 0;
	static int dim_x = 0;
	static int dim_y = 0;
	static int x_pad = 0;
	static int num_img = 1;
	static float speed = 1.0f;

	if (visible)
	{
		if (editor->getCurrentComponentList()[CUR_SPRITESHEET] != "")
		{


			int ss = editor->getCurrentSpriteSheet();
			Core::SpriteSheet* spritesheet = editor->getGamePtr()->getSpriteSheetFromID(ss);

			// Open controls popup
			ImGui::Begin("Sprite Finder", &visible);

			ImGui::PushItemWidth(200);
			ImGui::Text("Enter Sprite Name:");
			ImGui::InputText("  ", sprite_name, IM_ARRAYSIZE(sprite_name));

			ImGui::PushItemWidth(200);
			ImGui::Text("Set Sprite ID:");
			ImGui::InputText(" ", spriteIDInput, IM_ARRAYSIZE(spriteIDInput), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_CharsNoBlank);
			ImGui::Text(editor->getCurrentComponentList()[CUR_SPRITE].c_str());
			int spriteID = -1;
			if (spriteIDInput[0] != 0)
			{
				spriteID = atoi(spriteIDInput);
			}


			ImGui::Text("Location: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_x", &loc_x, 0, spritesheet->getDimensions().x);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_y", &loc_y, 0, spritesheet->getDimensions().y);
			ImGui::Text("Dimensions: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_x", &dim_x, 0, spritesheet->getDimensions().x / num_img);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_y", &dim_y, 0, spritesheet->getDimensions().y);
			ImGui::Text("x padding: ");
			ImGui::SameLine();
			ImGui::SliderInt("##x_pad", &x_pad, 0, 50);
			ImGui::Text("Number of Sprites: ");
			ImGui::SameLine();
			ImGui::SliderInt("##num_img", &num_img, 1, 30);
			ImGui::Text("Loop length (in seconds): ");
			ImGui::SameLine();
			ImGui::SliderFloat("##speed", &speed, 0.0f, 3.0f);

			if (ImGui::Button("Create")) {
				if (num_img == 1) {
					editor->getGamePtr()->createPartialSprite(sprite_name, spriteID, ss, glm::ivec2(loc_x, loc_y), glm::ivec2(dim_x, dim_y));
				} 				else if (num_img > 1) {
					editor->getGamePtr()->createLoopingSprite(sprite_name, spriteID, ss, num_img, speed, glm::ivec2(loc_x, loc_y), glm::ivec2(dim_x, dim_y), x_pad);
				}
			}

			// Set the new information of the window's highligher
			mSpriteHighlighter.setDimensions(glm::ivec2(dim_x, dim_y));
			mSpriteHighlighter.setLocation(glm::ivec2(loc_x, loc_y));
			mSpriteHighlighter.setNumImages(num_img);
			mSpriteHighlighter.setPadding(x_pad);

			ImGui::End();




			// set the windows default size (not needed for game window)
			//ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


			// the game view window itself
			ImGui::Begin("SpriteSheet View", &visible, ImGuiWindowFlags_NoTitleBar);
			updateWindowFocus();

			// Get size of drawable space on the window, instead of the entire size of the window
			ImVec2 canvas_size = ImGui::GetContentRegionAvail();

			glBindTexture(GL_TEXTURE_2D, mSpriteSheetTexCBO);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spritesheet->getDimensions().x, spritesheet->getDimensions().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glBindTexture(GL_TEXTURE_2D, 0);
			glViewport(0, 0, spritesheet->getDimensions().x, spritesheet->getDimensions().y); // Set viewport to the Game dimensions
			glBindFramebuffer(GL_FRAMEBUFFER, mSpriteSheetFBO);

			editor->getGamePtr()->renderSpriteSheet(spritesheet);
			mSpriteHighlighter.highlight();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glViewport(0, 0, (int)canvas_size.x, (int)canvas_size.y); // Reset viewport size // this line doesn't matter
			ImGui::Image((ImTextureID)mSpriteSheetTexCBO, ImVec2(canvas_size.x, canvas_size.y), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::End();
			ImGui::PopStyleVar();
		}
	}
}
