#pragma once

#include <cstdio>
#include <ctime>
#include <vector>
#include <memory> // For unique_ptr
#include <string>

#include <SDL.h>
#include <nlohmann/json.hpp>

#include <Page.h>
#include <Entity.h>
#include "MapPage.h"

namespace Core
{
    class Game
    {
    public:
        static int width;
        static int height;

        Game ( GLuint *o );
        Game ();
        Game ( std::string gameName );
        Game ( nlohmann::json &json );

        std::string GetGameName ();
        void SetGameName ( std::string newName );

        std::string GetAuthor ();
        void SetAuthor ( std::string newAuthor );

        std::string GetVersion ();
        void SetVersion ( std::string newVersion );

        std::string GetLMTime ();
        void SetLMTime ();
        void SetLMTime ( std::string time );

        std::string GetNote ();
        void SetNote ( std::string newNote );
        int AddNote ( std::string moreNote );

        /*
         * next sprint: multi pages
        int addDsipalyList ( int pageID );
        std::vector<int> getDisplayListID ();
        */

        // File loading and serialization
        static Game *parse ( nlohmann::json &root );
        nlohmann::json *serialize ();

        // from core team
        void init ();
        void initShader ();
        void sdl_die ( const char *err_msg );
        void handleInput ( SDL_Event event );
        void render ();
        void run (); // main entry

        int coreMain ( int argc, char *argv[] );




        // =========================
        // CONSTRUCTOR

        // =========================
        // ATTRIBUTES OPERATION

        // =========================
        // PROPERTY OPERATION

        // =========================
        // MEMBER OPERATION
        Page *addPage ( Page *p );
        Page *createPage ( std::string n );
        MapPage *createMapPage ( std::string, Map * );
        MapPage *createMapPage ( std::string );
        void deletePage ( Page * );
        void deletePage ( std::string );
        // =========================
        // UTILITY OPERATION
        void destroy ();
        void mainLoop ();
        void s1test ();
        std::vector <Page *> *getPageList ();
        Page *currentPage = nullptr;
       // from core team end
    private:
        Entity *entityInteractive;
        Entity *entityTallThin;
        Entity *entityShortWide;
        Entity *entityVeryShortWide;
        Entity *entityOrigin;
        Page *entityPage;
        MapPage *mapPage1;
        MapPage *mapPage2;

        std::string gameName;
        std::string author;
        std::string version;
        std::string lMTime; // last modified time
        std::string note;
        GLuint *texcbo;
        bool useFramebuffer;
        GLuint fbo;
        std::vector<Page *> pageList;
        // for current stage, remove in sprint 2
        // next sprint
        // std::vector<int> inDisplayList;

        SDL_Window *window; // Window of this Game
        SDL_GLContext gl_context; // The context of this Game
        unsigned int shaderProgram; // The shaders, set by initShaders before entering the game loop
    };
}

