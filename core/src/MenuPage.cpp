#include "MenuPage.h"

namespace Core {
    MenuPage::MenuPage(std::string s, Menu* menu) :
        Page(s), mMenu(menu)
    {
        mFont = new Font();
    }

    // Sets mMenu to the Menu created by the user
    void MenuPage::setMenu(Menu* menu) {
        // Set the new menu
        mMenu = menu;
    }

    // Returns mMenu
    Menu* MenuPage::getMenu() {
        return mMenu;
    }

    // Render the current Menu!
    void MenuPage::render() {
        // Make sure the menu exists
        if (mMenu) {
            int SPACING = 20;
            int ypos = Game::height;

            // Loop through all menu entries in the menu
            for (MenuEntry* entry : mMenu->mMenuEntries) {
                // Render the current menu entry
                int xpos = 50;
                ypos -= (entry->getSize() * 48);


                // If the text is not empty
                if (entry->getText().compare("")) {
                    // Draw text
                    // Add return value to xpos, because renderText returns the number of pixels the text is wide. 
                    xpos += mFont->renderText(entry->getText(), glm::ivec2(xpos, ypos), entry->getSize(), glm::vec3(1.0f, 0.0f, 1.0f));
                    xpos += SPACING;
                    xpos += mFont->renderText("ooooh", glm::ivec2(xpos, ypos), entry->getSize(), glm::vec3(0.0f, 0.0f, 1.0f));
                    xpos += SPACING;
                    xpos += mFont->renderText("aaaah", glm::ivec2(xpos, ypos), entry->getSize(), glm::vec3(1.0f, 0.0f, 0.0f));
                }

                // Get button callbacks
                void* cb1 = entry->getButton1Callback();
                void* cb2 = entry->getButton2Callback();

                // If both buttons exist
                if (cb1 && cb2) {
                    // TODO draw both buttons
                }
                // If only one button exists
                else if (cb1 && !cb2) {
                    // TODO draw one button
                }
            }
        }
        else {
            printf("No Menu is set for this MenuPage :(\n");
        }
    }
}