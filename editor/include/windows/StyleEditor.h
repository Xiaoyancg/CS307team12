#pragma once

#include "windows/Window.h"

enum ImGuiStyleEnum {
    ImGuiStyle_DefaultClassic = 0,
    ImGuiStyle_DefaultDark,
    ImGuiStyle_DefaultLight,

    ImGuiStyle_Gray,        // (mine) This is the default theme of my main.cpp demo.
    ImGuiStyle_Light,       // (mine)
    ImGuiStyle_BlackCodz01, // Posted by @codz01 here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_DarkCodz01,  // Posted by @codz01 here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_GrayCodz01,  // Posted by @codz01 here: https://github.com/ocornut/imgui/issues/1607 (hope I can use it)
    ImGuiStyle_Purple,      // Posted by @fallrisk here: https://github.com/ocornut/imgui/issues/1607  (hope I can use it)
    ImGuiStyle_Cherry,      // Posted by @r-lyeh here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_DarkOpaque,  // (mine)
    ImGuiStyle_Soft,        // Posted by @olekristensen here: https://github.com/ocornut/imgui/issues/539 (hope I can use it)
    ImGuiStyle_EdinBlack,   // Posted (via image) by edin_p in the screenshot section of Dear ImGui
    ImGuiStyle_EdinWhite,   // Posted (via image) by edin_p in the screenshot section of Dear ImGui
    ImGuiStyle_Maya,        // Posted by @ongamex here https://gist.github.com/ongamex/4ee36fb23d6c527939d0f4ba72144d29
    ImGuiStyle_LightGreen,  // Posted by @ebachard here: https://github.com/ocornut/imgui/pull/1776 (hope I can use it)
    ImGuiStyle_Design,      // Posted by @usernameiwantedwasalreadytaken here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_Dracula,     // Posted by @ice1000 here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_Greenish,    // Posted by @dertseha here: https://github.com/ocornut/imgui/issues/1902 (hope I can use it)
    ImGuiStyle_C64,         // Posted by @Nullious here: https://gist.github.com/Nullious/2d598963b346c49fa4500ca16b8e5c67 (hope I can use it)
    ImGuiStyle_PhotoStore,  // Posted by @Derydoca here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_CorporateGreyFlat,   // Posted by @malamanteau here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_CorporateGreyFramed, // Posted by @malamanteau here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_VisualDark, // Posted by @mnurzia here: https://github.com/ocornut/imgui/issues/2529 (hope I can use it)
    ImGuiStyle_SteamingLife, // Posted by @metasprite here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_SoftLife, // Just a quick variation of the ImGuiStyle_SteamingLife style
    ImGuiStyle_GoldenBlack, // Posted by @CookiePLMonster here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_Windowed,               // Badly adapted from the Win98-DearImgui customization made by @JakeCoxon in his fork here https://github.com/JakeCoxon/imgui-win98 (hope I can use it)
    ImGuiStyle_OverShiftedBlack,    // Posted by @OverShifted here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_AieKickGreenBlue,    // Posted by @aiekick here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)
    ImGuiStyle_AieKickRedDark,    // Posted by @aiekick here: https://github.com/ocornut/imgui/issues/707 (hope I can use it)

    ImGuiStyle_DarkOpaqueInverse,
    ImGuiStyle_GrayCodz01Inverse,
    ImGuiStyle_PurpleInverse,
    ImGuiStyle_LightGreenInverse,
    ImGuiStyle_DesignInverse,

    ImGuiStyle_Custom,

    ImGuiStyle_Count
};

class StyleEditor : public Window {
public:
	DEFAULT_CONSTRUCTOR(StyleEditor) {}
	void draw();
};