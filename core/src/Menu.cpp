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

    Button* Menu::getButtonFromClick(float x, float y) {
        for (Button* button : mButtons) {
            float* coords = button->getCoords();
            glm::vec2 btn_min(coords[0], coords[3]);
            glm::vec2 btn_max(coords[4], coords[1]);
            if ((btn_min.x <= x) && (x <= btn_max.x) &&
                (btn_min.y <= y) && (y <= btn_max.y)) {
                return button;
            }
        }

        return nullptr;
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
