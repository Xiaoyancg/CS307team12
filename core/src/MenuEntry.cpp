#pragma once
#include "MenuEntry.h"

namespace Core {

	MenuEntry::MenuEntry(std::string text, float size, void* b1, void* b2) :
		mText(text),
		mSize(size)
	{

		// TODO Calculate width of text
		

		mButton1.callback = b1;
		mButton1[0] = ;
		mButton1[1] = ;
		mButton1[2] = 0;
		mButton1[3] = 1;
		mButton1[4] = ;
		mButton1[5] = ;
		mButton1[6] = 0;
		mButton1[7] = 0;
		mButton1[8] = ;
		mButton1[9] = ;
		mButton1[10] = 1;
		mButton1[11] = 1;
		mButton1[12] = ;
		mButton1[13] = ;
		mButton1[14] = 1;
		mButton1[15] = 0;
			

		mButton2.callback = b1;


		mSize = size / 48;
	};

	std::string MenuEntry::getText() {
		return mText;
	}
	float MenuEntry::getSize() {
		return mSize;
	}
	void* MenuEntry::getButton1Callback() {
		return mButton1.callback;
	}
	void* MenuEntry::getButton2Callback() {
		return mButton2.callback;
	}

	void MenuEntry::debugPrint() {
		printf("%s %p %p\n", mText.c_str(), mButton1.callback, mButton2.callback);
	}
}