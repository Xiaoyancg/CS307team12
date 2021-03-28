#include "MenuPage.h"


namespace Core {
    // Sets mMenu to the Menu created by the user
    void MenuPage::setMenu(Menu* menu) {
        // Set the old menu to have no associated page
        if (mMenu) {
            mMenu->mAssociatedPage = nullptr;
        }

        // Set the new menu
        mMenu = menu;

        // Set the new menu's associated page
        if (mMenu != nullptr) {
            mMenu->mAssociatedPage = this;
        }
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
                printf("MenuEntry %p\n", entry);
            }
        }
        else {
            printf("No Menu is set for this MenuPage :(\n");
        }
    }
}