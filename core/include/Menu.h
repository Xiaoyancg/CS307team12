#pragma once
#pragma warning(disable : 5033)
#include <vector>
#include "MenuEntry.h"


// Menu class
namespace Core
{
	class Menu
	{
	public:
		// Takes a scale, the number of tiles in the x and y direction
		Menu(std::string name);
		~Menu();

		void setName(std::string);
		std::string getName();

		void setRowSize(int size);

		// A MenuEntry consists of some text, and two buttons at most.
		// If a button function pointer is null, then that button will not be created or rendered
		MenuEntry* createMenuEntry(std::string entryText, void* buttonFunc1);
		MenuEntry* createMenuEntry(std::string entryText, void *buttonFunc1, void *buttonFunc2);



	private:
		// This will hold an arbitrary number of MenuEntry objects.
		// A MenuEntry is just text with a button or two (depending on what is specified by the user).
		// Each entry will be rendered on a separate line of the MenuPage based on mRowSize
		std::vector<MenuEntry*> mMenuEntries;

		int mRowSize;

		std::string mMenuName;

		friend class MenuPage;

	};
}