#pragma once
#include "Page.h"
#include "Sprite.h"
#include "Menu.h"
#include <string>

namespace Core
{
    // MenuPage class
    class MenuPage : public Page
    {
    public:

        // Needs the current window to make a new opengl context, in the Page constructor 
        MenuPage::MenuPage() :
            MenuPage("No MenuPage name :(")
        { }

        MenuPage::MenuPage(std::string s) :
            MenuPage(s, new Menu("No Menu name :("))
        { }

        MenuPage::MapPage(std::string s, Menu* menu) :
            Page(s), mMenu(menu)
        { }


        // Sets mMenu to the Menu created by the user
        void setMenu(Menu* menu);

        // Returns mMenu
        Menu* getMenu();

        // Render the current Menu!
        void render();

    private:
        Menu* mMenu;

        // These 2 sprites are used on increment/decrement buttons of MenuEntry objects
        static Sprite* mSpriteInc;
        static Sprite* mSpriteDec;
    };
}