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
        // =========================
        // UTILITY OPERATION

        void mainLoop ();


       // from core team end
    private:

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
        Page *currentPage = nullptr;
        // next sprint
        // std::vector<int> inDisplayList;

        SDL_Window *window; // Window of this Game
        SDL_GLContext gl_context; // The context of this Game
        unsigned int shaderProgram; // The shaders, set by initShaders before entering the game loop
    };
}

