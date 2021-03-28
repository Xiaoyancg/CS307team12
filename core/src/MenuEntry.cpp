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
}