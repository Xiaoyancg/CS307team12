#pragma once
#include <string>
#include "Menu.h"

namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Menu::Menu(std::string name) : 
        mAssociatedPage(nullptr),
        mMenuName(name)
    { }

    Menu::~Menu()
    {
        // TODO delete mMenuEntries

        // If the map is deleted while being used in a MapPage
        if (mAssociatedPage != nullptr)
        {
            mAssociatedPage->setMenu(nullptr);
        }
    }

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
    MenuEntry* createMenuEntry(std::string entryText, void* buttonFunc1()) {
        return new MenuEntry(entryText, buttonFunc1, nullptr);
    }
    MenuEntry* createMenuEntry(std::string entryText, void* buttonFunc1(), void* buttonFunc2()) {
        return new MenuEntry(entryText, buttonFunc1, buttonFunc2);
    }
}
