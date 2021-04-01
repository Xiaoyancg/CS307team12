/*
#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Font.h"
#include "Menu.h"

// MenuEntry class (nested class of Menu)
namespace Core
{
	class Menu::MenuEntry
	{
	public:
		Menu::MenuEntry(std::string text, float size, void* b1, void* b2);

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

		Font* mFont;

		void render();
	};
}
*/