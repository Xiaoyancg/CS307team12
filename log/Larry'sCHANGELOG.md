# Larry's Changelog
All notable changes to this project will be documented in this file.
# Meeting memo
- move mGameSprite to Core
# TODO:
- pointer deletion
- json
- script
- logic
  - keyup
- signal
- add test function in editor
  - make a new sdl context
  - when that window should close set the current context back
- unittest for everything
- add a timer logic 
- render text
- better cmake
- a sprite initializer after initShader
- move set/get to header (done game)
- move constructor to header
- logic family get/set/constructor
- a resource class and a interface class for editor
- Json 
- include order
- when no sprite, do something

# Note:
- use typedef
- private class member has prefix m instead of _
- Spent 10 hours trying to ignore warning from glm, failed
- C5033 is level 1 warning can't 
- also .iln warning is tricky to ignore
- Reconsider the role of core and vm
  - Core has to handle logic, where logic contain input event. So the core has
    to know the context. It's redundant to init sdl and pass the context to VM,
    since core can just do all stuff. I didn't consider the event handler thing,
    I thought core can be a render. Now, I think we don't actually need the vm
    thing. But since we have that in the backlog, we just make a place holder
    with the functionality of finding the gdata file.
- There's a linker error when linking soil to unittest.
  - It's due to the conflict in c++ runtime library. soil is using another flag.
    I did search some solution but it doesn't solve the problem. However, the
    problem will magically disappear and turns into warning if restart the IDE
    or console. If it works, it works. 
- Use google c++ style guide for commenting
  - Declaration comments describe use of the function (when it is non-obvious);
    comments at the definition of a function describe operation.
- Tried some inherit way, the best way is to use pointer and union
- Don't use pointer too much, use it only when required.
- the game is still oop but resource is not oop
  - DBMS
- Dependency Injection is a way to achieve Inversion of Control, objected
  oriented programming.
- std libraries use heap.
- Sometimes can't use forward declaration because we forward declaration can't
  tell compiler how big the type is.
- if switch page, have to remove all the signal or logic or script from the list
- narrow the target in sprint 2
  




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
  - logic is a container of response and Action, evoke is in Action
    - action has a class for each kind of action
    - Esignal | Response | Action
    - order of logics
    1. add signals (from the script in last loop, from sdl event)
    2. traverse signal list
       1. for each signal, go through all the corresponding type of response. e.g. for each key signal, go through all key response.
       2. When a response meets it's defined condition, set the logic to ready.
    3. Traverse Logic list in order (some logic should execute before others).
       When a logic is in ready, evoke action.
  - changed design use this one below
     ```    
        for each (signal in signalList) {
             switch (signal.type) {
                 case key:
                     for each (logic in inputLogicList){
                         logic.check(signal)
                     }
                     break;
             }
         }
  - Do we really need a signal class?
  - wait a moment. I can let game execute the script can I?
    - Script is just some parameter that let game run
- OOP
  - Actually we can do oop, with DI
    - Render with id in tile and entity 
    - Let game handle script. 
    - Let game handle resource. 
- why use spriteManager: 
    - to let entity handle rendering, we need to pass a reference of texture id to entity class, we can either pass a instance of data or a manager, manager is better.



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
- Example sprite 1,2.png
- Sprite::mFilename, Sprite::mSpriteID, and associated operation 
- Sprite::(name,filename, ID)
- serialize of sprites
- parse of sprites in Game and SpriteManager::parse(json)
- init mGameSprite in setupSpriteReference
- s2sprint
- searchAllFileWithExtension
- Action, MoveEntityConstantly
- Response
- Esignal
- getTimeString() in VMTool
- struct Core::GameState and GameResource in Game.h
- json: 
  - root: LogicList, StartState, SpriteList, NumberOfSprites
- mnumSprites in SpriteManager
- struct EditorParam in Game.h
- gstate/gresource/geditorPara 
- SpriteManager::memptyIDQ
- practice/testCpp
- base class for script, logic and signal
- logic manager
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
- searchGdataFile to searchFileWithExtension
- separate Game::createSprite
- move all output to myexe in build to remove the error from mix library location
- glsl 1-y
- Esignal -> Signal
- Game.h include everything
- rearrange include order or logic family
- remove logic
- rename response to logic
- rename signaltype to eventtype
- rename SignalEvent to SignalVariant
- rename action to script to avoid conflict from the undo/redo
- moved width and height to Core::gwidth and gheight
- remove getLMT() in Game
- moved every member variable in game to gstate/gresource/geditorParam
- memptyIDQ -> memptyIDV
- remove signal manager
- msprite to std::unordered_map<std::string, Sprite*>
- make sprite only refer to string spriteName
- use string as reference everywhere

### Removed
- __plitr and __pl define in Game.h
- Core::Game();


