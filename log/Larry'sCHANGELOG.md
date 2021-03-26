# Larry's Changelog
All notable changes to this project will be documented in this file.
# TODO:
- pointer deletion
# Note:
- use typedef
- private class member has prefix m instead of _
- Spent 10 hours trying to ignore warning from glm, failed
- C5033 is level 1 warning can't 
- also .iln warning is tricky to ignore
### Added
- practice/demo_of_ImGui_Docking
- CMakeList new subdirectory demo_of_ImGui_Docking
- new class design
- new mockup ui
- typedef Core::PLitr, Core::PL
- empty glm.cpp in core/src for glm library
- Core::RenderTarget class not in use rn
- bool Game::editorMode;
- string gameName in Editor.cpp 
- bool isSaved in Editor.cpp
- set isSaved after open/ save
- class Script in Script.h, Script.cpp
- class Logic in Logic.h, Logic.cpp
- enum class LogicT in Logic.h 
- //Core::Game *game in Game.h
- Game::Game(json) for VM
- searchGdata() in VMTool
### Fixed
- typo
- multiple warnings besides the type conversion and void *
- Editor.cpp save menu item
- Editor.cpp save as popup
- Editor openDialog
- type conversion in map, use ivec2 instead of vec2
- function naming convention 
- json instance error, use json::parse instead of constructor
### Changed
- typedef name change
- make glm a library instead of interface to get rid of warnings
- add comment in CMakeList
- change the target include rules
- rename Game::currPage
- GameVMMain


### Removed
- __plitr and __pl define in Game.h
- Core::Game();


