#pragma once

#include "windows/Window.h"
#include "Tile.h"
#include <glad/glad.h>

class MapWindow : public Window {
  public:
	DEFAULT_CONSTRUCTOR(MapWindow) {
		glGenTextures(1, &mMapTexCBO);
		glGenFramebuffers(1, &mMapFBO);
		glBindTexture(GL_TEXTURE_2D, mMapTexCBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mMapFBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMapTexCBO, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void draw();
	Core::Tile* handleClick();
  private:
	GLuint mMapTexCBO;
	GLuint mMapFBO;
};
