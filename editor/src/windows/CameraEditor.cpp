#include "windows/CameraEditor.h"
#include "Entity.h"

void CameraEditor::draw() {
	// Camera editor
	Core::Game* game = editor->getGamePtr();

	static int x_lock_bound = 0;
	static int y_lock_bound = 0;
	static float zoom = 1.0f;
	bool none_selected = false;

	if (visible && game) {
		// set the windows default size
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		// map editor
		if (ImGui::Begin("Camera Editor", &visible)) {
			ImGui::Text("Zoom: ");
			ImGui::PushItemWidth(100);
			ImGui::SameLine();
			ImGui::SliderFloat("##zoom", &zoom, 0.0f, 5);
			if (game->getCamera()) {
				game->getCamera()->setZoom(zoom);
			}


			ImGui::Text("Locked Entity");
			ImGui::SameLine();

			// Get the name of the locked entity
			std::string name;
			Core::Entity* temp;
			if ((temp = game->getCurrPage()->getEntityWithID(game->getCameraEntityID()))) {
				name = temp->getName();
			}
			else {
				name = "No locked Entity...";
				none_selected = true;
			}

			// Show list of possible Entities to lock to
			if (ImGui::BeginCombo("##lockedEntity", name.c_str(), 0)) {
				if (game->getCurrPage()) {
					const bool isSelected = (game->getCameraEntityID() == CAMERA_NO_LOCKED_ENTITY);
					if (ImGui::Selectable("None", isSelected)) {
						game->setCameraEntityID(-2);
						none_selected = true;
					}

					for (auto entity : game->getCurrPage()->getEntityList()) {
						int id = entity->getEntityId();
						// Only show entities if they have an assigned ID
						if (id >= 0) {
							const bool isSelected = (id == game->getCameraEntityID());
							if (ImGui::Selectable(entity->getName().c_str(), isSelected)) {
								game->setCameraEntityID(entity->getEntityId());
							}
							if (isSelected) {
								ImGui::SetItemDefaultFocus();
							}
						}
					}

					ImGui::EndCombo();
				}
			}

			if (!none_selected) {
				// Set Bounds
				ImGui::Text("Entity Lock Bounds: ");
				ImGui::PushItemWidth(100);
				ImGui::SameLine();
				ImGui::SliderInt("##x_bound", &x_lock_bound, 0, Core::Game::width / 2);
				ImGui::SameLine();
				ImGui::SliderInt("##y_bound", &y_lock_bound, 0, Core::Game::height / 2);

				// Set Button
				if (ImGui::Button("Set Bounds")) {
					game->setCameraEntityBounds(x_lock_bound, y_lock_bound);
				}
			}
		}


		ImGui::End();
	}
}
