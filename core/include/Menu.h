#pragma once
#pragma warning(disable : 5033)
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Font.h"
#include "TextBox.h"
#include "Button.h"

//#include "Game.h" // for Game::width and Game::height

// Menu class
namespace Core
{
	class Menu
	{
	public:
		// Takes a scale, the number of tiles in the x and y direction
		Menu(std::string name);
		Menu(const Menu& other) :
			mFont(other.mFont),
			mTextBoxes(other.mTextBoxes),
			mButtons(other.mButtons),
			mMenuName(other.mMenuName)
		{}
		~Menu();

		void setName(std::string);
		std::string getName();
		void setFont(Font *);

		// A MenuEntry consists of some text, and two buttons at most.
		// If a button function pointer is null, then that button will not be created or rendered
		void createTextBox(std::string text, float size, glm::vec2 location, glm::vec3 color);
		void createButton(std::string text, float size, glm::vec3 textColor, glm::vec2 location, glm::vec2 dimensions, glm::vec3 buttonColor);

		Button* getButtonFromClick(float x, float y);

		// Menu rendering, called by MenuPage::render()
		void render();

	private:

		Font *mFont;

		std::vector<TextBox *> mTextBoxes;
		std::vector<Button *> mButtons;

		std::string mMenuName;
	};
}