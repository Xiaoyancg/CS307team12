#pragma once

#include "windows/Window.h"

class PageEditor : public Window
{
public:
	DEFAULT_CONSTRUCTOR(PageEditor) {}
	void draw();

private:
	int page_id = -1;
};
