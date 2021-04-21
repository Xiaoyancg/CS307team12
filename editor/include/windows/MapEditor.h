#pragma once

#include "windows/Window.h"

enum class EditMode {
	Collision,
	Sprite
};

class MapEditor : public Window {
public:
	DEFAULT_CONSTRUCTOR(MapEditor) {}
	void draw();
	void drawCreateMapPopup();
	EditMode getEditMode() {
		return mode;
	}
	int getSelectedSpriteID() {
		return mSelectedSpriteID;
	}
private:
	EditMode mode = EditMode::Sprite;
	int mSelectedSpriteID = -1;
};
