#pragma once
#include <string>
#include <glm/glm.hpp>
#include "Font.h"


namespace Core {
	// Used by Buttons on Menu.h
	// This could also be used on its own to render text if you have a Font
	class TextBox
	{
	public:
		TextBox(std::string text, Font*& font, int size, glm::vec2 location, glm::vec3 color) :
			text(text), mFont(font), size(size), location(location), color(color)
		{}
		void render() {
			mFont->renderText(text, location, size, color);
		}

		// All of these would have getters/setters so im just making them public
		std::string text;
		int size;
		glm::vec2 location;
		glm::vec3 color;

	private:
		Font*& mFont;
	};

}