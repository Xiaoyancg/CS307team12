#pragma once

#include <cstdio>
#include <ctime>
#include <vector>
#include <memory> // For unique_ptr
#include <string>
#include <iterator>
#include <SDL.h>
#include <nlohmann/json.hpp>

// Parchment header
#include "RenderTarget.h"
#include "Page.h"
#include "Entity.h"
#include "MapPage.h"

// page list iterator
#define __plitr std::vector<Page *>::iterator
#define __pl std::vector<Page *>
namespace Core
{
    typedef std::vector<Page *>::iterator PLitr;
    typedef std::vector<Page *> PL;

    //Game *game; // init in editor or vm, used by logic

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
        Game(GLuint *o);
        Game(std::string gameName);
        Game(nlohmann::json &json, GLuint *o);

        // =========================
        // ATTRIBUTES OPERATION

        std::string getGameName();
        void SetGameName(std::string newName);

        std::string getAuthor();
        void SetAuthor(std::string newAuthor);

        std::string getVersion();
        void SetVersion(std::string newVersion);

        std::string getLMTime();
        void SetLMTime();
        void SetLMTime(std::string time);

        std::string getNote();
        void SetNote(std::string newNote);
        int AddNote(std::string moreNote);

        // File loading and serialization
        Game *parse(nlohmann::json &root);
        nlohmann::json *serialize();

        // from core team

        // =========================
        // PROPERTY OPERATION

        // =========================
        // MEMBER OPERATION
        Page *addPage(Page *p);
        Page *createPage(std::string n);
        MapPage *createMapPage(std::string, Map *);
        MapPage *createMapPage(std::string);
        MapPage *createMapPage();
        void deletePage(Page *);
        void deletePage(std::string);
        std::vector<Page *> *getPageList();
        int getNumPage();

        // =========================
        // STATE OPERATION

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
        int moveCurrentPage(std::vector<Page *>::iterator target);

        // =========================
        // UTILITY OPERATION

        // init SDL
        void init();
        void initShader();
        void sdl_die(const char *err_msg);
        void handleInput(SDL_Event event);
        void render();

        // main entry for vm
        void run();

        // clean context after loop end
        void destroy();

        // main game loop
        void mainLoop();

    private:
        // =========================
        // UTILITY OPERATION

        // check the iterator not begin
        bool _isBegin(PLitr i);
        bool _isBeforeEnd(PLitr i);

        // ==========================
        // ATTRIBUTES VARIABLE

        std::string gameName;
        std::string author;
        std::string version;
        // last modified time
        std::string lMTime;
        std::string note;

        // ===========================
        // STATE VARIABLES

        Entity *currCtrlEntity;
        bool useFramebuffer;

        // page list iterator: current page iterator
        PLitr _currPitr;

        // condition of game is rendering to editor
        // if true use cbo
        bool editorMode;

        // =========================u=
        // MEMBER VARIABLES

        Page *currPage = nullptr;
        GLuint *texcbo;
        GLuint fbo;
        std::vector<Page *> pageList;

        std::vector<int> inDisplayList;

        SDL_Window *window;         // Window of this Game
        SDL_GLContext gl_context;   // The context of this Game
        unsigned int shaderProgram; // The shaders, set by initShaders before entering the game loop
    };
}
