#pragma once

#include "windows/Window.h"


class SpriteSheetWindow : public Window {
public:
	DEFAULT_CONSTRUCTOR(SpriteSheetWindow) {
		glGenTextures(1, &mSpriteSheetTexCBO);
		glGenFramebuffers(1, &mSpriteSheetFBO);
		glBindTexture(GL_TEXTURE_2D, mSpriteSheetTexCBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mSpriteSheetFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSpriteSheetTexCBO, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void draw();
private:
	GLuint mSpriteSheetTexCBO;
	GLuint mSpriteSheetFBO;
};
