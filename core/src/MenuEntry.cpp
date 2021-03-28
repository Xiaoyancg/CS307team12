#pragma once
#include "MenuEntry.h"

namespace Core {
	std::string MenuEntry::getText() {
		return mText;
	}
	void* MenuEntry::getButtonCallback1() {
		return mButtonCallback1;
	}
	void* MenuEntry::getButtonCallback2() {
		return mButtonCallback2;
	}

	void MenuEntry::debugPrint() {
		printf("%s %p %p\n", mText.c_str(), mButtonCallback1, mButtonCallback2);
	}
}