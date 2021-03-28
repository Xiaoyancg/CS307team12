#pragma once
#include <string>
#include "Menu.h"

namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Menu::Menu(std::string name) : 
        mMenuName(name)
    { }


    void Menu::setName(std::string name) {
        mMenuName = name;
    }
    std::string Menu::getName() {
        return mMenuName;
    }

    void Menu::setRowSize(int size) {
        mRowSize = size;
    }

    // A MenuEntry consists of some text, and two buttons at most.
    // If a button function pointer is null, then that button will not be created or rendered
    MenuEntry* Menu::createMenuEntry(std::string entryText, void* buttonFunc1) {
        MenuEntry* me = new MenuEntry(entryText, buttonFunc1, nullptr);
        mMenuEntries.push_back(me);
        return me;
    }
    MenuEntry* Menu::createMenuEntry(std::string entryText, void* buttonFunc1, void* buttonFunc2) {
        MenuEntry* me = new MenuEntry(entryText, buttonFunc1, buttonFunc2);
        mMenuEntries.push_back(me);

        return me;
    }
}
