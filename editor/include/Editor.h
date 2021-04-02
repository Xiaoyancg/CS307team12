#pragma once
#include <SDL.h>
#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <imfilebrowser.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <stb_image_aug.h>

int EditorMain();

enum SelectionEnum
{
	SAVEAS,
	DELETEPROJECT,
	GAMEVIEW,
	MAPVIEW,
	ENTITYEDITOR,
	PAGEEDITOR,
	MAPEDITOR,
	SAVEPOPUP,
	DELETEPOPUP,
	OBJECTTREE,
	SPRITEEDITOR,
	SCRIPTEDITOR,
	SPLASHSCREEN,

	// LEAVE THIS AT THE END OF THE LIST OR ELSE THIS IS A THREAT
	SELECT_COUNT,
};

enum CurrentComponent
{
	CUR_ENTITY,
	CUR_LOGIC,
	CUR_MAP,
	CUR_PAGE,
	CUR_SCRIPT,
	CUR_SPRITE,

	COMP_COUNT,
};