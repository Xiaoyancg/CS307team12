#pragma once
#include "Menu.h"


namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Menu::Menu(std::string name) : 
        mMenuName(name),
        mRowSize(48)
    { 
        mFont = new Font("../../../../core/res/comicz.ttf");
    }

    void Menu::setName(std::string name) {
        mMenuName = name;
    }
    std::string Menu::getName() {
        return mMenuName;
    }

    void Menu::setFont(Font* newFont) {
        mFont = newFont;
    }

    // A MenuEntry consists of some text, and two buttons at most.
    // If a button function pointer is null, then that button will not be created or rendered
    void Menu::createMenuEntry(std::string entryText, float size, void* buttonFunc1) {
        MenuEntry* me = new MenuEntry(entryText, mFont, size, buttonFunc1, nullptr);
        setMenuEntryCoords();
        mMenuEntries.push_back(me);
    }
    void Menu::createMenuEntry(std::string entryText, float size, void* buttonFunc1, void* buttonFunc2) {
        MenuEntry* me = new MenuEntry(entryText, mFont, size, buttonFunc1, buttonFunc2);
        setMenuEntryCoords();
        mMenuEntries.push_back(me);
    }

    // This will calculate the coordinates of both buttons in each entry of this Menu.
    // We have to calculate these to render and do collision detection with the mouse (eventually)
    void Menu::setMenuEntryCoords() {
        printf("Setting button coords...TODO\n");
        int coords[8];

    }


    void Menu::render() {
        // Loop through and render all menu entries in the menu
        for (MenuEntry* entry : mMenuEntries) {
            entry->render();
        }
    }
}
