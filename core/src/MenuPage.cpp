#include "MenuPage.h"

namespace Core {
    MenuPage::MenuPage(std::string s, Menu* menu) :
        Page(s), mMenu(menu)
    { }

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
            mMenu->render();
        }
        else {
            printf("No Menu is set for this MenuPage :(\n");
        }
    }
}