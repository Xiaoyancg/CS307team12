#pragma once

#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>
#include <string>
#include <iterator>
#include <any>
#include <variant>

#include <SDL.h>
#include <nlohmann/json.hpp>

// Parchment header
#include "Camera.h"
#include "Page.h"
#include "Entity.h"
#include "MapPage.h"
#include "RenderTarget.h"
#include "SpriteManager.h"
#include "MenuPage.h"
#include "LogicManager.h"

// page list iterator
#define __plitr std::vector<Page *>::iterator
#define __pl std::vector<Page *>
namespace Core
{

    typedef std::vector<Page *>::iterator PLitr;
    typedef std::vector<Page *> PL;

    //Game *game; // initContext in editor or vm, used by logic

    class Game
    {
    public:
        // =========================
        // PUBLIC VARIABLE
        static int width;
        static int height;

        // =========================
        // CONSTRUCTOR

        // Game ();
        Game() : Game("Untitled Game") {}
        Game(nlohmann::json &json);
        Game(std::string gameName);

        void initialize();

        // =========================
        // ATTRIBUTES OPERATION (attributes mean non functionality related variables)
        // Attributes are refering to additional information of an object. Properties are describing the characteristics of an object.

        std::string getGameName();
        void setGameName(std::string newName);

        std::string getAuthor();
        void setAuthor(std::string newAuthor);

        std::string getVersion();
        void setVersion(std::string newVersion);

        std::string getLMTime();
        void setLMTime();
        void setLMTime(std::string time);

        std::string getNote();
        void setNote(std::string newNote);
        int AddNote(std::string moreNote);

        // parse json to game data for both editor and vm
        Game *parse(nlohmann::json &root);
        // serialize the game data to json for usage in editor
        nlohmann::json *serialize();

        // =========================
        // PROPERTY OPERATION ( Properties are the variables that may change the performance, like the size of the window or IDK )

        // =========================
        // MEMBER OPERATION ( functions that targeting )

        Page *addPage(Page *p);
        Page *createPage(std::string n);
        MapPage *createMapPage(std::string, Map *);
        MapPage *createMapPage(std::string);
        MapPage *createMapPage();
        Map *createMapOnDefaultMapPage(std::string name, int cols, int rows, int tilesize);
        void deleteDefaultMapPageCurrentMap();
        MenuPage *createMenuPage(std::string, Menu *);
        MenuPage *createMenuPage(std::string);
        MenuPage *createMenuPage();
        void deletePage(Page *);
        void deletePage(std::string);
        std::vector<Page *>& getPageList();
        int getNumPage();

        // Sprite operations
        unsigned int createSprite(std::string, std::string);
        unsigned int createSprite(std::string, std::string, int);
        void deleteSprite(int);
        Sprite *getSpriteFromID(int);
        std::unordered_map<int, Sprite *> getSprites();

        // Map operations
        std::vector<Map *> getDefaultMapPageMaps();
        MapPage *getDefaultMapPage();
        void renderDefaultMapPage();

        // set the currpage pointer and iterator to target
        void setCurrentPage(Page *p);
        Page *getCurrPage();

        /// \brief Create a default Signal object
        /// *For editor, binding to create button in signal editor
        ///
        /// \return Signal*
        Signal *createSignal();

        /// \brief Create a default Script object
        /// *For editor, binding to create button in script editor
        ///
        /// \return Script*
        Script *createScript();

        /// \brief Create a default Logic object
        /// *For editor, binding to create button in logic editor
        ///
        /// \return Logic*
        Logic *createLogic();

        // For editor
        void deleteLogic(int logicId);
        void deleteSignal(int signalId);
        void deleteScript(int scriptId);

        // initContext SDL context
        void initContext();
        // init opengl related flag
        void initShader();
        glm::vec2 getShaderScale()
        {
            return shaderScale;
        }
        void setShaderScale(glm::vec2 scale);

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


        void onGameCreation();

    private:
        std::string gameName;
        std::string author;
        std::string version;
        // last modified time
        std::string lMTime;
        std::string note;


        Entity *currCtrlEntity;

        int scaleUniformID = -1;
        glm::vec2 shaderScale;


        // page list iterator: current page iterator
        PLitr _currPitr;

        // condition of game is rendering to editor
        // if true use cbo
        bool editorMode;

        LogicManager _logicManager;
        /* ---------------------------- MEMBER VARIABLES ---------------------------- */
        // Camera used to move map
        Camera* mCamera;

        // page pointer
        Page *currPage = nullptr;
        
        // contains all pages
        std::vector<Page *> pageList;

        // Each game needs at least one MapPage, so heres a pointer to the default one
        MapPage *mGameMapPage;

        // the current in display pagelist
        // FIXME: why use int?
        std::vector<int> inDisplayList;

        // the render window of this Game
        SDL_Window *window;

        // The context of this Game
        SDL_GLContext gl_context;

        // The shaders, set by initShaders before entering the game loop
        unsigned int shaderProgram;

        // FIXME: to Core
        // Holds pointers to all the game's sprites and handles ID's properly
        // Use gameSprites.createSprite(filename); to create the sprite from the file
        // Use gameSprites.atID(id); to get the pointer to the sprite with ID 'id'
        // Use gameSprites.deleteSprite(id); to delete the sprite with ID 'id'
        SpriteManager mGameSprites;
    };
}
