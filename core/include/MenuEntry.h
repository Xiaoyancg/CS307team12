#pragma once
#include <string>

// MenuEntry class
namespace Core
{
	class MenuEntry
	{
	public:
		MenuEntry(std::string text, float size, void* b1, void* b2);

		std::string getText();
		float getSize();
		void* getButton1Callback();
		void* getButton2Callback();

		void debugPrint();

	private:
		std::string mText;
		float mSize;

		struct Button {
			void* callback;
			int coords[16]; // 4 pairs of (x,y)
		};

		struct Button mButton1;
		struct Button mButton2;
	};
}