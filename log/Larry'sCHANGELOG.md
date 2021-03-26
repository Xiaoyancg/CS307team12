# Larry's Changelog
All notable changes to this project will be documented in this file.
# TODO:
- pointer deletion
- json
- script
- logic
- signal
- add test function in editor
  - make a new sdl context
  - when that window should close set the current context back
- unittest for everything
- add a timer logic 
- text
# Note:
- use typedef
- private class member has prefix m instead of _
- Spent 10 hours trying to ignore warning from glm, failed
- C5033 is level 1 warning can't 
- also .iln warning is tricky to ignore
- Reconsider the role of core and vm
  - Core has to handle logic, where logic contain input event. So the core has to know the context. It's redundant to init sdl and pass the context to VM, since core can just do all stuff. I didn't consider the event handler thing, I thought core can be a render. Now, I think we don't actually need the vm thing. But since we have that in the backlog, we just make a place holder with the functionality of finding the gdata file.
- There's a linker error when linking soil to unittest.
  - It's due to the conflict in c++ runtime library. soil is using another flag. I did search some solution but it doesn't solve the problem. However, the problem will magically disappear and turns into warning if restart the IDE or console. If it works, it works. 
- Use google c++ style guide for commenting
  - Declaration comments describe use of the function (when it is non-obvious); comments at the definition of a function describe operation.


### Design Notes
- Logic and signal
  - signal is a function to set a flag on the current game logic list ( or signal list )
  - Storage and linking way in json, the script can logics are connected by the id. in run time, they are bind by logic pointers
  - loop
    1. game state logic ( should close / mute ) 
    2. timer
    3. game content logic ( switch page, render entity, change new sprite)
    4. script logics ( damage )
    5. user inputs ( mouse key)
  

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
- timer.h, a header-only general timer class in core
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
- GameVMMain, search gdata file


### Removed
- __plitr and __pl define in Game.h
- Core::Game();


