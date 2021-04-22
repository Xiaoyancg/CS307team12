#include "windows/GameWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Button.h"

void GameWindow::handleMenuPageClick(glm::vec2 click)
{
    glm::mat4 ortho = glm::inverse(glm::ortho((float) -Core::Game::width/2, (float) Core::Game::width/2, (float)-Core::Game::height/2, (float) Core::Game::height/2));
    glm::vec4 normalized = glm::vec4(click.x * 2 - 1, click.y * 2 - 1, 0, 1);
    glm::vec4 scaled = ortho * normalized;

    Core::MenuPage* menupage = dynamic_cast<Core::MenuPage*>(editor->getGamePtr()->getCurrPage());

    // When a menu page is clicked, the clicked button will be found
    Core::Button* clickedButton = menupage->getMenu()->getButtonFromClick(scaled.x, scaled.y);
    if (clickedButton) {
        //printf("Button %p clicked!\n", clickedButton);
    }
}

void GameWindow::handleMapPageClick(glm::vec2 click)
{
    Core::Camera* cam = editor->getGamePtr()->getCamera();
    glm::vec2 projCoords = cam->projectToWorld(click);

    Core::MapPage* mappage = dynamic_cast<Core::MapPage*>(editor->getGamePtr()->getCurrPage());

    // TODO: Do something

}


void GameWindow::handleClick(float x, float y, float width, float height) {
    // Calculate the min/max coords of the MapView window
    // min/max are used to check for click detection within the coordinates of the window
    ImVec2 min = ImGui::GetWindowPos();
    min.x += x;
    min.y += y;
    ImVec2 max = min;
    max.x += width;
    max.y += height; // Get max y value of MapView
    
    // At this point, min and max contain the minimum and maximum coordinates of the window based on the top right corner as the origin

    //////////// Mouse handling within the MapView window ////////////
    // If mouse left is currently pressed and the MapView is focused
    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowFocused()) {
        // If click is within the coordinates of the MapView window (in global coords based on top right corner)
        ImVec2 click_pos = ImGui::GetMousePos();
        if ((min.x < click_pos.x && click_pos.x < max.x) && (min.y < click_pos.y && click_pos.y < max.y))
        {
            // This is the click on the screen scaled to (0<x,y<1) based on the bottom left corner.
            // This is what gets sent to the handlers, which will be unprojected to get its location without camera operations
            glm::vec2 scaledClick = glm::vec2(
                (click_pos.x - min.x) / width,
                1 - (click_pos.y - min.y) / height
            );

            // Check if there is a current page
            Core::Page* currPage = editor->getGamePtr()->getCurrPage();
            if (currPage) {
                // NOTE: This is where you can add click detection for other types of Pages. This is how to handle clicks for MenuPages
                if (dynamic_cast<Core::MenuPage*>(currPage)) { handleMenuPageClick(scaledClick); }
                else if (dynamic_cast<Core::MapPage*>(currPage)) { handleMapPageClick(scaledClick); } // page type isn't being saved yet? so this wont work yet
            }
        }
    }
}

void GameWindow::draw()
{
    if (visible)
    {
        // possibly implement a new function here for readability purposes
        if (editor->getGamePtr() != nullptr)
        {
            // set the windows default size (not needed for game window)
            //ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
            //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


            // the game view window itself
            ImGui::Begin("Game View", &visible);
            updateWindowFocus();

            if (!editor->isGameRunning()) {
                if (ImGui::Button("Run")) {
                    editor->runGame();
                }
            } else {
                if (ImGui::Button("Stop")) {
                    editor->stopGame();
                }
                // auto now = std::chrono::steady_clock::now();
                // float dt = (float) (now - lastTime).count() / std::chrono::steady_clock::period::den;
                // lastTime = now;
                editor->getGamePtr()->logicLoop();
            }
            
            ImGui::Separator();

            glViewport(0, 0, editor->getGamePtr()->width, editor->getGamePtr()->height); // Set viewport to the Game dimensions
            glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
            editor->getGamePtr()->render(); // Render Game with new viewport size
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            ImVec2 windowSize = ImGui::GetWindowSize();
            float aspectRatio = (float) editor->getGamePtr()->width / editor->getGamePtr()->height;
            ImVec2 cursor = ImGui::GetCursorPos();
            cursor.y += 10;
            float gameHeight = windowSize.y - cursor.y - 10;
            float gameWidth = gameHeight * aspectRatio;
            if (gameWidth + 20 > windowSize.x) {
                gameWidth = windowSize.x - 20;
                gameHeight = gameWidth / aspectRatio;
            }

            cursor.x = (windowSize.x - gameWidth) / 2;
            ImGui::SetCursorPos(cursor);
            ImGui::Image((ImTextureID) mTexCBO, ImVec2(gameWidth, gameHeight), ImVec2(0, 1), ImVec2(1, 0));

            handleClick(cursor.x, cursor.y, gameWidth, gameHeight); // Handle click with cursor value (Important!)

            ImGui::End();
        }
    }
}
