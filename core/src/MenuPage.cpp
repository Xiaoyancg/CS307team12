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
            // Loop through all menu entries in the menu
            for (MenuEntry* entry : mMenu->mMenuEntries) {
                // Render the current menu entry

                // If the text is not empty
                if (entry->getText().compare("")) {
                    // TODO Draw text
                    mFont->renderText("test", glm::ivec2(50, 50), 1, glm::vec3(1.0f, 0.0f, 1.0f));
                }

                // Get button callbacks
                void* cb1 = entry->getButtonCallback1();
                void* cb2 = entry->getButtonCallback2();

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