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
        MenuPage() :
            MenuPage("No MenuPage name :(")
        { }

        MenuPage(std::string s) :
            MenuPage(s, new Menu("No Menu name :("))
        { }

        MenuPage::MenuPage(std::string s, Menu* menu);


        // Sets mMenu to the Menu created by the user
        void setMenu(Menu* menu);

        // Returns mMenu
        Menu* getMenu();

        // Render the current Menu!
        void render();

        void parse(nlohmann::json& root);
        nlohmann::json serialize();

    private:
        Menu* mMenu;
    };
}