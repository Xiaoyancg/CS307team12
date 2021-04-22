#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Textbox.h"

namespace Core {
	class Button
	{

	public:
		Button(std::string text, Font*& font, float size, glm::vec3 textColor, glm::vec2 location, glm::vec2 dimensions, glm::vec3 buttonColor) :
			mLocation(location), mDimensions(dimensions), mButtonColor(buttonColor)
		{
			if (text != "") {
				mText = new TextBox(text, font, size, glm::vec2((location.x + dimensions.x / 2) - (font->calcTextWidth(text, size) / 2), (location.y + dimensions.y / 2) - (font->calcTextHeight(text, size) / 2)), textColor);
			} else {
				mText = nullptr;
			}

			mCoords[0] = mLocation.x;
			mCoords[1] = mLocation.y + mDimensions.y;
			mCoords[2] = mLocation.x;
			mCoords[3] = mLocation.y;
			mCoords[4] = mLocation.x + mDimensions.x;
			mCoords[5] = mLocation.y + mDimensions.y;
			mCoords[6] = mLocation.x + mDimensions.x;
			mCoords[7] = mLocation.y;
		}
		void render() {
			// Render button
			// Unbind the current sprite
			glBindTexture(GL_TEXTURE_2D, 0);

			GLint program;
			glGetIntegerv(GL_CURRENT_PROGRAM, &program);
			GLfloat color[] = { mButtonColor.x, mButtonColor.y, mButtonColor.z };
			glUniform3fv(glGetUniformLocation(program, "color"), 1, color);

			float coords[16];
			coords[0] = mCoords[0];
			coords[1] = mCoords[1];
			coords[2] = 0;
			coords[3] = 1;
			coords[4] = mCoords[2];
			coords[5] = mCoords[3];
			coords[6] = 0;
			coords[7] = 0;
			coords[8] = mCoords[4];
			coords[9] = mCoords[5];
			coords[10] = 1;
			coords[11] = 1;
			coords[12] = mCoords[6];
			coords[13] = mCoords[7];
			coords[14] = 1;
			coords[15] = 0;

			// Buffer and draw button
			// NOTE: Change the int multiplier whenever new data will be added to the shaders.
			//       Right now, there are 4 points (8 ints), with 4 texture points (8 ints) = 16 * sizeof(int)
			glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), coords, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


			// Render text on button
			if (mText) {
				mText->render();
			}
		}

		float* getCoords() {
			return mCoords;
		}

	private:
		glm::vec2 mLocation;
		glm::vec2 mDimensions;
		glm::vec3 mButtonColor;
		TextBox* mText;
		float mCoords[8];
	};
}