#include "windows/MenuPageEditor.h"
#include "MenuPage.h"

void MenuPageEditor::draw() {
	Core::Game* game = editor->getGamePtr();

	static char entry_text[128] = "";
	static int size = 0;
	static int loc_x = 0;
	static int loc_y = 0;
	static int dim_x = 0;
	static int dim_y = 0;

	if (visible && game)
	{
		Core::MenuPage* menuPage;
		if (editor->getCurrentComponentList()[CUR_PAGE] != "" && (menuPage = dynamic_cast<Core::MenuPage*>(editor->getGamePtr()->getCurrPage())))
		{
		

			// Menu editor popup
			ImGui::Begin("Menu Editor", &visible);

			// Text
			ImGui::PushItemWidth(200);
			ImGui::Text("Enter Item Text:");
			ImGui::InputText("  ", entry_text, IM_ARRAYSIZE(entry_text));


			// Size
			ImGui::Text("Text Size: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##size", &size, 0, 128);


			// Location
			ImGui::Text("Location: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_x", &loc_x, -Core::Game::width/2, Core::Game::width/2);
			ImGui::SameLine();
			ImGui::SliderInt("##loc_y", &loc_y, -Core::Game::height / 2, Core::Game::height/2);

			// Create text box
			if (ImGui::Button("Create Text Entry")) {
				game->createMenuTextBox(entry_text, size, glm::vec2(loc_x, loc_y), glm::vec3(.24, .52, .89));
			}

			ImGui::Text("Dimensions: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_x", &dim_x, 0, Core::Game::width);
			ImGui::SameLine();
			ImGui::SliderInt("##dim_y", &dim_y, 0, Core::Game::height);


			// Create text box
			if (ImGui::Button("Create Box Entry")) {
				game->createMenuButton(entry_text, size, glm::vec3(.24, .52, .89), glm::vec2(loc_x, loc_y), glm::vec2(dim_x, dim_y), glm::vec3(.24, .52, .89));
			}

			ImGui::End();
		}
	}
}
