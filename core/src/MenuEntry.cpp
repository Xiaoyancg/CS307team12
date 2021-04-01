#pragma once
#include "Menu.h"

namespace Core {

	Menu::MenuEntry::MenuEntry(std::string text, Font*& font, float size, void* b1, void* b2) :
		mText(text),
		mFont(font),
		mSize(size)
	{
		printf("New MenuEntry: %s\n", text.c_str());
		//int textWidth = mFont->calcTextWidth(text, size);

		// Set button callbacks and texture coordinates (we skip the position coords right now,
		// they will be set by Menu later.)
		// BUTTON 1
		mButton1.callback = b1;
		mButton1.coords[2] = 0;
		mButton1.coords[3] = 1;
		mButton1.coords[6] = 0;
		mButton1.coords[7] = 0;
		mButton1.coords[10] = 1;
		mButton1.coords[11] = 1;
		mButton1.coords[14] = 1;
		mButton1.coords[15] = 0;

		// BUTTON 2
		mButton2.callback = b2;
		mButton1.coords[2] = 0;
		mButton1.coords[3] = 1;
		mButton1.coords[6] = 0;
		mButton1.coords[7] = 0;
		mButton1.coords[10] = 1;
		mButton1.coords[11] = 1;
		mButton1.coords[14] = 1;
		mButton1.coords[15] = 0;

		// Font size is set to 48 by default, so if the user specifies:
		// size <48 font size will shrink
		// size 48 font size will not change
		// size >48 font size will grow
		// NOTE: Fonts will look goofy if you try to make them too large, because it's like upscaling a 'low-res' image
		mSize = size / 48;
	};

	std::string Menu::MenuEntry::getText() {
		return mText;
	}
	float Menu::MenuEntry::getSize() {
		return mSize;
	}
	void* Menu::MenuEntry::getButton1Callback() {
		return mButton1.callback;
	}
	void* Menu::MenuEntry::getButton2Callback() {
		return mButton2.callback;
	}

	void Menu::MenuEntry::setTextPos(int x, int y) {
		mTextPos.x = x;
		mTextPos.y = y;
	}

	// Sets the coordinates of the specified button (1 or 2) to the 8 ints stored in *coords
	void Menu::MenuEntry::setButtonCoords(int btn, int* coords) {
		struct Button* button;
		if (btn == 1) {
			button = &mButton1;
		}
		else if (btn == 2) {
			button = &mButton2;
		}
		else {
			printf("Menu::MenuEntry::setButtonCoords > uhhh..... you tried to modify a button that doesn't exist\n");
			return;
		}

		button->coords[0] = coords[0];
		button->coords[1] = coords[1];
		button->coords[4] = coords[2];
		button->coords[5] = coords[3];
		button->coords[8] = coords[4];
		button->coords[9] = coords[5];
		button->coords[12] = coords[6];
		button->coords[13] = coords[7];
	}

	void Menu::MenuEntry::debugPrint() {
		printf("%s %p %p\n", mText.c_str(), mButton1.callback, mButton2.callback);
	}

	void Menu::MenuEntry::render() {
		// If the text is not empty
		if (mText.compare("")) {
			// Draw text
			mFont->renderText(mText, mTextPos, mSize, glm::vec3(1.0f, 0.0f, 1.0f));
		}

		if (mButton1.callback) {
			printf("b1 has callback\n");
		}

		if (mButton2.callback) {
			printf("b2 has callback\n");
		}
	}
}