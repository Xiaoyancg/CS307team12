#pragma once
#include <glad/glad.h>
#include "Menu.h"

namespace Core {

	Menu::MenuEntry::MenuEntry(std::string text, Font*& font, int size, void* b1, void* b2) :
		mText(text),
		mFont(font)
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
		mButton2.coords[2] = 0;
		mButton2.coords[3] = 1;
		mButton2.coords[6] = 0;
		mButton2.coords[7] = 0;
		mButton2.coords[10] = 1;
		mButton2.coords[11] = 1;
		mButton2.coords[14] = 1;
		mButton2.coords[15] = 0;

		// Font size is set to 48 by default, so if the user specifies:
		// size <48 font size will shrink
		// size 48 font size will not change
		// size >48 font size will grow
		// NOTE: Fonts will look goofy if you try to make them too large, because it's like upscaling a 'low-res' image
		mSize = (float) size / 48.0f;
	};

	std::string Menu::MenuEntry::getText() {
		return mText;
	}
	int Menu::MenuEntry::getSize() {
		return mSize * 48; // Multiplied by default font size (48) to get size in pixels 
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

		/*for (int i = 0; i < 16; i++) {
			printf("%d ", button->coords[i]);
			if (i == 3 || i == 7 || i == 11 || i == 15) {
				printf("\n");
			}
		}*/
	}

	int Menu::MenuEntry::getTextWidth() {
		printf("with size: %f\n", mSize);
		return mFont->calcTextWidth(mText, mSize);
	}

	int Menu::MenuEntry::getTextHeight() {
		return mFont->calcTextHeight(mText, mSize);
	}

	void Menu::MenuEntry::debugPrint() {
		printf("%s %p %p\n", mText.c_str(), mButton1.callback, mButton2.callback);
	}

	void Menu::MenuEntry::render() {
		// If the text is not empty
		if (mText.compare("")) {
			// Draw text
			mFont->renderText(mText, mTextPos, mSize, glm::vec3(1.0f, 1.0f, 1.0f));
		}

		if (mButton1.callback) {
			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * 16, vertices);
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(int), mButton1.coords, GL_DYNAMIC_DRAW);

			// render quad
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			if (mButton2.callback) {
				//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * 16, vertices);
				glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(int), mButton2.coords, GL_DYNAMIC_DRAW);

				// render quad
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			}
		}
	}
}