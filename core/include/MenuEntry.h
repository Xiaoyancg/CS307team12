#pragma once
#include <string>

// MenuEntry class
namespace Core
{
	class MenuEntry
	{
	public:
		MenuEntry(std::string text, void* b1, void* b2) :
			mText(text),
			mButtonCallback1(b1),
			mButtonCallback2(b2)
		{ };

		std::string getText();
		void* getButtonCallback1();
		void* getButtonCallback2();

		void debugPrint();

	private:
		std::string mText;
		void* mButtonCallback1;
		void* mButtonCallback2;
	};
}