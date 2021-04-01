#pragma once
#pragma warning(disable : 5033)
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Font.h"
//#include "Game.h" // for Game::width and Game::height


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


		// A MenuEntry consists of some text, and two buttons at most.
		// If a button function pointer is null, then that button will not be created or rendered
		void createMenuEntry(std::string entryText, float size, void* buttonFunc1);
		void createMenuEntry(std::string entryText, float size, void *buttonFunc1, void *buttonFunc2);

		void setMenuEntryCoords();


		// Menu rendering, called by MenuPage::render()
		void render();

	private:
		// MenuEntry is a nested class of Menu because MenuEntry objects shouldn't be used on their own,
		// they should only ever be used within a Menu by using Menu::createMenuEntry
		class Menu::MenuEntry
		{
		public:
			MenuEntry(std::string text, Font*& font, float size, void* b1, void* b2);

			std::string getText();
			float getSize();
			void* getButton1Callback();
			void* getButton2Callback();

			void setTextPos(int x, int y);
			void setButtonCoords(int btn, int* coords);

			void debugPrint();

			void render();

		private:
			std::string mText;
			glm::ivec2 mTextPos; // The lower left corner of the text
			float mSize;

			struct Button {
				void* callback;
				int coords[16]; // 4 pairs of (x,y)
			};

			struct Button mButton1;
			struct Button mButton2;

			Font*& mFont; // This gets set to whatever this Menu's mFont is.
		};

		Font* mFont;

		int mRowSize;

		// This will hold an arbitrary number of MenuEntry objects.
		// A MenuEntry is just text with a button or two (depending on what is specified by the user).
		// Each entry will be rendered on a separate line of the MenuPage based on mRowSize
		std::vector<MenuEntry*> mMenuEntries;

		std::string mMenuName;
	};
}