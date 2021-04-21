#pragma once

#include "windows/Window.h"
#include "Tile.h"
#include "Camera.h"
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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glGenTextures(1, &mTintTexture);
		glBindTexture(GL_TEXTURE_2D, mTintTexture);
        GLubyte texData[] = {0, 0, 0, 128};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

		glBindTexture(GL_TEXTURE_2D, 0);

		cam = new Core::Camera;
	}
	~MapWindow() {
		delete cam;
	}
	void draw();
	Core::Tile* handleClick();
  private:
	Core::Tile* lastTile = nullptr;
	GLuint mMapTexCBO;
	GLuint mTintTexture;
	GLuint mMapFBO;
	Core::Camera* cam;
};
