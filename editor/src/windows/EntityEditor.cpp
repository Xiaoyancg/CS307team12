#include "windows/EntityEditor.h"
#include "HelpMarker.h"
#include "UndoRedo.h"

extern void HelpMarker(const char *);

void EntityEditor::draw()
{
	if (visible)
	{
		// set the windows default size
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Entity Editor", &visible))
		{
			Core::Page *currPage = editor->getGamePtr()->getCurrPage();

			if (ImGui::Button("Create New Entity"))
			{
				ImGui::OpenPopup("create_entity_popup");
			}

			if (ImGui::BeginPopup("create_entity_popup"))
			{
				ImGui::PushItemWidth(200);
				ImGui::Text("Enter Entity Name:");
				ImGui::InputText("##entity_name", entityName,
								 IM_ARRAYSIZE(entityName));
				if (ImGui::Button("Create"))
				{
					if (strlen(entityName) != 0)
					{
						std::string e = entityName;
						auto action = [this, e]() {
							editor->getGamePtr()->getCurrPage()->createEntity(
								e);
						};
						auto restore = [this, e]() {
							editor->getGamePtr()->getCurrPage()->deleteEntity(
								e);
						};
						pushAction(action, restore);
						action();
						// ENDUNDO
						// clear the buffer after use
						entityName[0] = '\0';
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			static int current_entity = 0;
			if (ImGui::BeginListBox(
					"##entityListBox",
					ImVec2(200, 8 * ImGui::GetTextLineHeightWithSpacing())))
			{
				// Set default selected entity to be the first in the entity
				// list
				if (currentEntityIdx == -1 &&
					currPage->getEntityList().size() > 0)
				{
					currentEntityIdx = 0;
				}

				for (int i = 0; i < currPage->getEntityList().size(); i++)
				{
					auto entity = currPage->getEntityList()[i];
					bool is_selected = (currentEntityIdx == i);
					if (ImGui::Selectable(entity->getName().c_str(),
										  is_selected))
					{
						currentEntityIdx = i;
						x_pos = entity->getLocation().x;
						y_pos = entity->getLocation().y;
						entity_id = entity->getEntityId();
						inScriptId = entity->getInScriptId();
						entity_width = entity->getScale().x;
						entity_height = entity->getScale().y;
						sprite_id = entity->getSpriteID();
						invisible = entity->isInvisible();
					}

					// Set the initial focus when opening the combo (scrolling +
					// keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}

			if (ImGui::Button("Change Name") && currentEntityIdx != -1)
			{
				getCurrentEntity()->setName(entityName);
				// clear the buffer after use
				entityName[0] = '\0';
			}
			ImGui::SameLine();

			if (ImGui::Button("Delete Entity") && currentEntityIdx != -1)
			{
				int idx = -1;
				size_t original =
					editor->getGamePtr()->getCurrPage()->getEntityList().size();
				auto &eList = currPage->getEntityList();
				for (int i = 0; i < eList.size(); i++)
				{
					if (i == currentEntityIdx)
					{
						idx = i;
						break;
					}
				}
				if (idx > -1)
				{
					Core::Entity savedEntity(*eList[idx]);
					auto action = [this]() {
						editor->getGamePtr()->getCurrPage()->deleteEntity(getCurrentEntity());
					};
					auto restore = [this, idx, savedEntity]() {
						Core::Entity *newEntity = new Core::Entity(savedEntity);
						editor->getGamePtr()
							->getCurrPage()
							->getEntityList()
							.insert(editor->getGamePtr()
											->getCurrPage()
											->getEntityList()
											.begin() +
										idx,
									newEntity);
						currentEntityIdx = idx;
					};
					// Only delete if the entity was found

					action();

					if (currPage->getEntityList().size() < original)
					{
						currentEntityIdx = eList.size() > 0 ? 0 : -1;
						editor->showDeleteSuccessPopup();
						pushAction(action, restore); // UNDO
						// memset to clear the buffer after use
						entityName[0] = '\0';
					}
				}
			}

			// get x pos and y pos for entity's new location
			if (currentEntityIdx == -1)
			{
				x_pos = 0;
				y_pos = 0;
			}
			else
			{

				ImGui::NewLine();
				ImGui::Text("Position: %d, %d", x_pos, y_pos);
				ImGui::Text("Scale: %d, %d", entity_width, entity_height);

				ImGui::Text("");
				ImGui::Text("X Pos:");
				ImGui::PushItemWidth(80);
				ImGui::SameLine();
				ImGui::InputInt("##x_pos", &x_pos);
				ImGui::SameLine();
				HelpMarker("Choose a value between [-10000,10000]");
				ImGui::Text("Y Pos:");
				ImGui::SameLine();
				ImGui::InputInt("##y_pos", &y_pos);
				ImGui::SameLine();
				HelpMarker("Choose a value between [-10000,10000]");
				if (ImGui::Button("Change Entity Position"))
				{
					// these are totally arbitrary #s to try to ensure entities
					// cant be rendered outside the game view window when at max
					// size should probably change to use monitor display
					// settings later on
					if (x_pos >= -10000 && x_pos <= 10000 && y_pos >= -10000 &&
						y_pos <= 10000)
					{
						for (Core::Entity *e : currPage->getEntityList())
						{
							if (e == getCurrentEntity())
							{
								// set location as specified by user
								// UNDO
								auto pos = glm::vec2(x_pos, y_pos);
								auto currentPos = e->getLocation();
								auto action = [e, pos]() {
									e->setLocation(pos);
								};
								auto restore = [e, currentPos]() {
									e->setLocation(currentPos);
								};
								pushAction(action, restore);
								action();
								// END UNDO
								break;
							}
						}
					}
					else
					{
						x_pos = 0;
						y_pos = 0;
					}
				}

				// set size
				// get width and height for entity's new size
				if (currentEntityIdx == -1)
				{
					entity_width = 0;
					entity_height = 0;
				}

				ImGui::NewLine();
				ImGui::Text("Width:");
				ImGui::PushItemWidth(80);
				ImGui::SameLine();
				ImGui::InputInt("##entity_width", &entity_width);
				ImGui::SameLine();
				HelpMarker("Choose a value between [0,1000]");
				ImGui::Text("Height:");
				ImGui::SameLine();
				ImGui::InputInt("##entity_height", &entity_height);
				ImGui::SameLine();
				HelpMarker("Choose a value between [0,1000]");
				if (ImGui::Button("Change Entity Size"))
				{
					// these are totally arbitrary #s to try to ensure entities
					// cant be rendered outside the game view window when at max
					// size should probably change to use monitor display
					// settings later on
					if (entity_width >= 0 && entity_width <= 1000 &&
						entity_height >= 0 && entity_height <= 1000)
					{
						for (Core::Entity *e : currPage->getEntityList())
						{
							if (e == getCurrentEntity())
							{
								// set location as specified by user
								// UNDO
								auto newScale =
									glm::vec2(entity_width, entity_height);
								auto currentScale = e->getScale();
								auto action = [e, newScale]() {
									e->setScale(newScale);
								};
								auto restore = [e, currentScale]() {
									e->setScale(currentScale);
								};
								pushAction(action, restore);
								action();
								// END UNDO
								break;
							}
						}
					}
					else
					{
						entity_width = 0;
						entity_height = 0;
					}
				}
				ImGui::NewLine();
				ImGui::Checkbox("Invisible", &invisible);
				if (getCurrentEntity() != nullptr)
				{
					getCurrentEntity()->setInvisible(invisible);
					ImGui::NewLine();
					if (!getCurrentEntity()->isInvisible())
					{
						ImGui::Text("Sprite:");
						ImGui::SameLine();
						std::string spriteLabel = "";
						auto &sprites = editor->getGamePtr()->getSprites();
						auto currentEntity = getCurrentEntity();
						if (currentEntity->getSpriteID() == -100)
						{
							spriteLabel = "Default";
						}
						else if (sprites.find(currentEntity->getSpriteID()) != sprites.end())
						{
							spriteLabel = sprites[currentEntity->getSpriteID()]->getName();
						}
						if (ImGui::BeginCombo("##entitySprite", spriteLabel.c_str()))
						{
							if (ImGui::Selectable("Default", currentEntity->getSpriteID() == -100))
							{
								int newID = -100;
								int currentID = currentEntity->getSpriteID();
								auto action = [this, newID]() {
									getCurrentEntity()->setSpriteID(newID);
								};
								auto restore = [this, currentID]() {
									getCurrentEntity()->setSpriteID(currentID);
								};
								pushAction(action, restore);
								action();
							}
							for (auto [id, sprite] : sprites)
							{
								bool selected = (id == currentEntity->getSpriteID());
								if (ImGui::Selectable(sprite->getName().c_str(), selected))
								{
									int newID = id;
									int currentID = currentEntity->getSpriteID();
									auto action = [this, newID]() {
										getCurrentEntity()->setSpriteID(newID);
									};
									auto restore = [this, currentID]() {
										getCurrentEntity()->setSpriteID(currentID);
									};
									pushAction(action, restore);
									action();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::InputInt("entity id", &entity_id);
						if (ImGui::Button("update Entity id"))
						{
							editor->getGamePtr()->getCurrPage()->getEntityList().at(currentEntityIdx)->setEntityId(entity_id);
						}
						ImGui::InputInt("in script id", &inScriptId);
						if (ImGui::Button("update in script id"))
						{
							editor->getGamePtr()->getCurrPage()->getEntityList().at(currentEntityIdx)->setInScriptId(inScriptId);
						}
					}
				}
			}
		}
		ImGui::End();
	}
}
