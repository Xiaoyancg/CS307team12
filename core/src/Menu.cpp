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

    // A MenuEntry consists of some text, and two buttons at most.
    // If a button function pointer is null, then that button will not be created or rendered
    void Menu::createMenuEntry(std::string entryText, int size, void *buttonFunc1)
    {
        MenuEntry *me = new MenuEntry(entryText, mFont, size, buttonFunc1, nullptr);
        mMenuEntries.push_back(me);
        setMenuEntryCoords();
    }
    void Menu::createMenuEntry(std::string entryText, int size, void *buttonFunc1, void *buttonFunc2)
    {
        MenuEntry *me = new MenuEntry(entryText, mFont, size, buttonFunc1, buttonFunc2);
        mMenuEntries.push_back(me);
        setMenuEntryCoords();
    }

    // This will calculate the coordinates of both buttons in each entry of this Menu.
    // We have to calculate these to render and do collision detection with the mouse (eventually)
    void Menu::setMenuEntryCoords()
    {
        int xpadding = 20;
        int ypadding = 20;
        int xspacing = 20;
        int yspacing = 20;

        int xpos = xpadding;
        int ypos = Game::height - ypadding; // Y starts at top of screen because the menu is rendered from top to bottom

        int coords[8];

        for (MenuEntry *entry : mMenuEntries)
        {
            // The width/height of the buttons, either the text height or the given height if the text is empty
            int buttonDim = 0;

            // Get coord for text position, set x and y
            xpos = xpadding;
            if (entry->getText().compare(""))
            {
                buttonDim = entry->getTextHeight();
            }
            else
            {
                buttonDim = entry->getSize();
            }
            ypos -= buttonDim; // Make room for the height of the button (which will fit the text size if needed)

            // Set the text position coord
            entry->setTextPos(xpos, ypos);

            // Set X location on screen for first button
            if (entry->getText().compare(""))
            {
                xpos += entry->getTextWidth();
                xpos += xspacing;
            }

            // Set coords for b1 if it exists
            if (entry->getButton1Callback())
            {
                coords[0] = xpos;
                coords[1] = ypos + buttonDim;
                coords[2] = xpos;
                coords[3] = ypos;
                coords[4] = xpos + buttonDim;
                coords[5] = ypos + buttonDim;
                coords[6] = xpos + buttonDim;
                coords[7] = ypos;
                entry->setButtonCoords(1, coords);

                xpos += buttonDim; // Skip width of button
                xpos += xspacing;  // Give spacing between button1 and button2

                // Set coords for b2 if it exists (and b1 exists too)
                if (entry->getButton2Callback())
                {
                    coords[0] = xpos;
                    coords[1] = ypos + buttonDim;
                    coords[2] = xpos;
                    coords[3] = ypos;
                    coords[4] = xpos + buttonDim;
                    coords[5] = ypos + buttonDim;
                    coords[6] = xpos + buttonDim;
                    coords[7] = ypos;
                    entry->setButtonCoords(2, coords);
                }
            }

            ypos -= yspacing; // Give spacing between rows
        }
    }

    void Menu::render()
    {
        // Loop through and render all menu entries in the menu
        for (MenuEntry *entry : mMenuEntries)
        {
            entry->render();
        }
    }
}
