#pragma once
#include "Menu.h"
#include "Game.h"

namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Menu::Menu(std::string name) : mMenuName(name)
    {
        mFont = new Font("./comicz.ttf");
    }

    void Menu::setName(std::string name)
    {
        mMenuName = name;
    }
    std::string Menu::getName()
    {
        return mMenuName;
    }

    void Menu::setFont(Font *newFont)
    {
        mFont = newFont;
    }

    void Menu::createTextBox(std::string text, float size, glm::vec2 location, glm::vec3 color) {
        mTextBoxes.push_back(new TextBox(text, mFont, size, location, color));
    }
    void Menu::createButton(std::string text, float size, glm::vec3 textColor, glm::vec2 location, glm::vec2 dimensions, glm::vec3 buttonColor) {
        mButtons.push_back(new Button(text, mFont, size, textColor, location, dimensions, buttonColor));
    }

    void Menu::render()
    {
        // Loop through and render all menu entries in the menu
        for (Button *button: mButtons)
        {
            button->render();
        }

        for (TextBox *textbox : mTextBoxes) {
            textbox->render();
        }
    }
}
