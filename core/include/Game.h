#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <iterator>
#include <any>
#include <variant>

#include <SDL.h>
#include <nlohmann/json.hpp>

// Parchment header
#include "Page.h"
#include "Entity.h"
#include "MapPage.h"
#include "RenderTarget.h"
#include "SpriteManager.h"
#include "Logic.h"

// page list iterator

///
/// @brief the package contains class game and global game variables
///
///
namespace Core
{

    struct GameState
    {
        std::vector<Logic *> *keyLogicList;
        std::vector<Logic *> *mouseLogicList;
        std::vector<Logic *> *timerLogicList;
        std::vector<Logic *> *directLogicList;
        std::vector<Logic *> *scriptList;
        std::vector<Signal> *signalList;
        int width = 0, height = 0;
        // page pointer
        Page *currPage = nullptr;

        Entity *currCtrlEntity; // the current in display pagelist

        // FIXME: why use int?
        std::vector<int> inDisplayList;

        // the render window of this Game
        SDL_Window *window;

        // The context of this Game
        SDL_GLContext gl_context;

        // The shaders, set by initShaders before entering the game loop
        unsigned int shaderProgram;

        /// True if program is running in editor, use framebuffer
        /// False means in vm, render to window
        bool editorMode;
    };

    struct GameResource
    {
        SpriteManager *spriteManager;

        // contains all pages
        std::vector<Page *> *pageList;
    };

    struct EditorParam
    {
        // texcbo from editor
        GLuint *texcbo;
        // framebuffer object
        GLuint fbo;
    };
    extern struct GameState gstate;
    extern struct GameResource gresource;
    extern struct EditorParam geditorParam;
    // forward declaration
    class Game;
    extern SpriteManager gGameSprites = SpriteManager::SpriteManager();
    // g stands for global variable
    extern std::vector<Logic *> *gkeyLogicList;
    extern std::vector<Logic *> *gmouseLogicList;
    extern std::vector<Logic *> *gtimerLogicList;
    extern std::vector<Logic *> *gdirectLogicList;
    extern std::vector<Logic *> *gscriptList;
    // the signalList that game loop checks
    extern std::vector<Signal> signalList;

    /// @brief The parchment game engine core component. The game runtime object
    /// takes the role of rendering, event handling, game data file loading and
    /// serializing, and running the game with one call.
    /// @note game doesn't contain runtime states, the game state variable is in
    /// core as global variable
    class Game
    {
    public:
        //* ------------------------- CONSTRUCTOR ------------------------ *//

        // TODO: check empty page condition in editor mode and add new page
        // function in editor
        // Editor new
        Game(std::string gameName, GLuint *texcbo)
            : mgameName(gameName)
        {
            Core::geditorParam.texcbo = texcbo;
            gstate.editorMode = true;
        }

        // Editor open
        Game(nlohmann::json &json, GLuint *texcbo)
        {
            this->parse(json);
            Core::geditorParam.texcbo = texcbo;
        }

        // VM
        Game(nlohmann::json &json) { this->parse(json); }

        //* ---------------- ATTRIBUTES OPERATION ---------------- *//
        //  (attributes mean non functionality related variables)
        // Attributes are refering to additional information of an object.
        // Properties are describing the characteristics of an object.

        std::string getGameName() { return this->mgameName; }
        void setGameName(std::string gameName) { this->mgameName = gameName; }

        std::string getAuthor() { return this->mauthor; }
        void setAuthor(std::string author) { this->mauthor = author; }

        std::string getVersion() { return this->mversion; }
        void setVersion(std::string version) { this->mversion = version; }

        std::string getLMTime() { return this->mLMTime; }
        void setLMTime(std::string LMTTime) { this->mLMTime = LMTTime; }

        std::string getNote() { return this->note; }
        void setNote(std::string newNote) { this->note = newNote; }

        //* ----------------- PROPERTY OPERATION ----------------- *//
        // ( Properties are the variables that may change the performance,
        // like the size of the window or IDK )

        //* ------------------ MEMBER OPERATION ------------------ *//
        // ( functions that targeting members variables )

        Page *addPage(Page *p);
        Page *createPage(std::string n);
        MapPage *createMapPage(std::string, Map *);
        MapPage *createMapPage(std::string);
        MapPage *createMapPage();
        void deletePage(Page *);
        void deletePage(std::string);
        std::vector<Page *> *getPageList();
        int getNumPage();

        // Sprite operations
        unsigned int createSprite(std::string, std::string);
        unsigned int createSprite(std::string, std::string, int);
        void deleteSprite(int);
        Sprite *getSpriteFromID(int);
        std::unordered_map<int, Sprite *> getSprites();

        //* ------------------- STATE OPERATION ------------------ *//
        // ( the flags and pointers that describing the current state of
        // performance )

        // set the currpage pointer and iterator to target
        void setCurrentPage(Page *p);
        Page *getCurrPage();
        Entity *setCurrCtrlEntity(Entity *);
        Entity *getCurrCtrlEntity();

        /// <summary>
        /// Move current page pointer and iterator
        /// to the target iterator in the pageList.
        /// <para>Can be fail ( begin(), end() out of range)</para>
        /// </summary>
        /// <param name="target">the target pagelist iterator</param>
        /// <returns>0 if fail</returns>

        // TODO: depreciate iterator
        int moveCurrentPage(std::vector<Page *>::iterator target);

        //* ------------------ UTILITY OPERATION ----------------- *//

        ///
        /// @brief parse json to game data for both editor and vm
        ///
        /// @param root : nlohmann::json
        /// @return Game*
        ///
        Game *parse(nlohmann::json &root);

        ///
        /// @brief serialize the game data to json for usage in editor
        ///
        /// @return nlohmann::json*
        ///
        nlohmann::json *serialize();

        // initContext SDL context
        void initContext();

        // init opengl related flag
        void initShader();

        void sdl_die(const char *err_msg);
        void handleInput(SDL_Event event);

        // render the all currpage with entities
        void render();

        // main entry for vm, deals everything, call after constructed instance
        // handle exit
        void run();

        // clean context
        void destroy();

        // the only game loop
        void mainLoop();

    private:
        //* ------------------ UTILITY OPERATION ----------------- *//

        // ATTRIBUTES VARIABLE

        std::string mgameName;
        std::string mauthor;
        std::string mversion;
        // last modified time
        std::string mLMTime;
        std::string note;

        //* ------------------ MEMBER VARIABLES ------------------ *//

        // FIXME: to Core
        // Holds pointers to all the game's sprites and handles ID's properly
        // Use gameSprites.createSprite(filename); to create the sprite from the file
        // Use gameSprites.atID(id); to get the pointer to the sprite with ID 'id'
        // Use gameSprites.deleteSprite(id); to delete the sprite with ID 'id'
    };
}
