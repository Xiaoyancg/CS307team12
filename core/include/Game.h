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
#include "PageManager.h"
#include "Map.h"
#include "Entity.h"
#include "MapPage.h"
#include "RenderTarget.h"
#include "SpriteManager.h"
#include "Logic.h"

// page list iterator

/// @brief the package contains game class
namespace Core
{

    /// @brief The parchment game engine core component. The game runtime object
    /// takes the role of rendering, event handling, game data file loading and
    /// serializing, and running the game with one call.
    /// @note game doesn't contain runtime states, the game state variable is in
    /// core as global variable
    class Game
    {
    public:
        //* ----------------- ANCHOR CONSTRUCTOR ----------------- *//

        // TODO: check empty page condition in editor mode and add new page
        // function in editor
        // Editor new
        Game(std::string gameName, GLuint *texcbo)
            : mgameName(gameName), mtexcbo(texcbo), meditorMode(true)
        {
            mpageManager = PageManager(mwidth, mheight);
        }

        // Editor open
        Game(nlohmann::json &json, GLuint *texcbo) : mtexcbo(texcbo)
        {
            parse(json);
            mpageManager = PageManager(mwidth, mheight);
        }

        // VM
        Game(nlohmann::json &json)
        {
            parse(json);
            mpageManager = PageManager(mwidth, mheight);
        }

        // test
        Game() { mpageManager = PageManager(mwidth, mheight); }

        //* ------------- ANCHOR ATTRIBUTES OPERATION ------------ *//

        //  (attributes mean non functionality related variables)
        // Attributes are refering to additional information of an object.
        // Properties are describing the characteristics of an object.

        std::string getGameName() { return this->mgameName; }
        void setGameName(std::string gameName) { mgameName = gameName; }

        std::string getAuthor() { return this->mauthor; }
        void setAuthor(std::string author) { this->mauthor = author; }

        std::string getVersion() { return this->mversion; }
        void setVersion(std::string version) { this->mversion = version; }

        std::string getLMTime() { return this->mLMTime; }
        void setLMTime(std::string LMTTime) { this->mLMTime = LMTTime; }

        std::string getNote() { return this->mnote; }
        void setNote(std::string newNote) { this->mnote = newNote; }

        //* --------------- ANCHOR MEMBER OPERATION -------------- *//

        // these functions are for editors
        // *page
        int addPage(Page *page) { return mpageManager.addPage(page); }
        int createPage(std::string &pageName_ref)
        {
            return mpageManager.createPage(pageName_ref);
        }
        int createMapPage(std::string &pageName_ref, Map *map_ptr)
        {
            return mpageManager.createMapPage(pageName_ref, map_ptr);
        }
        int createMapPage(std::string &pageName_ref)
        {
            return mpageManager.createMapPage(pageName_ref);
        }
        int deletePage(Page *page_ptr)
        {
            return mpageManager.deletePage(page_ptr);
        }
        int deletePage(std::string &pageName_ref)
        {
            return mpageManager.deletePage(pageName_ref);
        }
        std::unordered_map<std::string, Page *> getPages()
        {
            return mpageManager.getPages();
        }
        std::unordered_map<std::string, Page *> getDisplayList()
        {
            return mpageManager.getDisplayList();
        }

        // *sprite
        /// \brief delete a sprite by name
        /// \param spriteName_ref
        /// \return int 0 for success, 1 for not found
        int deleteSprite(std::string &spriteName_ref)
        {
            return mspriteManager.deleteSprite(spriteName_ref);
        }
        Sprite *getSpriteByName(std::string &spriteName)
        {
            return mspriteManager.atName(spriteName);
        }
        /// \brief Get the Sprites, all sprites
        /// \return std::unordered_map<std::string, Sprite *>
        std::unordered_map<std::string, Sprite *> getSprites()
        {
            return mspriteManager.getSprites();
        }

        /// \brief Create a Sprite object and add it to game
        /// \param spriteName_ref
        /// \param fileName_ref
        /// \return int 0 success, 1 used name
        int createSprite(std::string &spriteName_ref,
                         std::string &fileName_ref)
        {
            return mspriteManager.createSprite(spriteName_ref,
                                               fileName_ref);
        }

        //* -------------- ANCHOR UTILITY OPERATION -------------- *//

        /// \brief parse json to game data for both editor and vm
        /// \param root : nlohmann::json
        /// \return Game*
        Game *parse(nlohmann::json &root);

        /// \brief serialize the game data to json for usage in editor
        /// \return nlohmann::json*
        nlohmann::json *serialize();

        // main entry for vm, deals everything, call after constructed instance
        // handle exit
        void run();

        // initContext SDL context
        void initContext();

        // init opengl related flag
        void initShader();

        // the only game loop
        void mainLoop();

        // render the all currpage with entities
        void render();

        // delete sdl variables
        void sdl_die(const char *err_msg);

        // clean context
        void destroy();

    private:
        //* ------------- ANCHOR ATTRIBUTES VARIABLE ------------- *//

        std::string mgameName;
        std::string mauthor;
        std::string mversion;
        // last modified time
        std::string mLMTime;
        std::string mnote;

        //* --------------- ANCHOR RENDER VARIABLES -------------- *//

        // texcbo from editor
        GLuint *mtexcbo;

        // framebuffer object
        GLuint mfbo;

        // The context of this Game
        SDL_GLContext mgl_context;

        // size of the window
        int mwidth = 800, mheight = 600;

        // the render window of this Game
        SDL_Window *mwindow;

        /// True if program is running in editor, use framebuffer
        /// False means in vm, render to window
        bool meditorMode = false;

        // The shaders, set by initShaders before entering the game loop
        unsigned int mshaderProgram;

        //* ----------------- ANCHOR KAREN KILLER ---------------- *//

        // conceptural and physical resource both use managers:
        // Pages, Logic( signal, script, Logic ), sprite and music
        SpriteManager mspriteManager;
        PageManager mpageManager;
    };
}
