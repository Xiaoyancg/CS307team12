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


namespace Core
{
    class Game
    {
    public:
        Game() {}
        Game( std::string gameName ) : gameName(gameName) {}

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

        // create a page in the page list
        // return the page id
        int CreatePage ();
        int CreatePage ( std::string pageName,
                        /*place holder change to enum class in sprint 2*/
                         std::string pageType );

        int GetPageID ( std::string pageName );
        int DeletePage ( int pageID );
        /*
         * next sprint: multi pages
        int addDsipalyList ( int pageID );
        std::vector<int> getDisplayListID ();
        */

        // File loading and serialization
        static std::unique_ptr<Game> parse(nlohmann::json& root);
        std::unique_ptr<nlohmann::json> serialize();

        // from core team
        void init ();
        void initShader ();
        void sdl_die ( const char *err_msg );
        void render ();
        void run (); // main entry

        int coreMain ( int argc, char *argv[] );





       // from core team end
    private:
        std::string gameName;
        std::string author;
        std::string version;
        std::string lMTime; // last modified time
        std::string note;

        std::vector<Page> pageList;
        // for current stage, remove in sprint 2
        int currentPage;
        // next sprint
        // std::vector<int> inDisplayList;

        SDL_Window *window; // Window of this Game
        SDL_GLContext gl_context; // The context of this Game
        unsigned int shaderProgram; // The shaders, set by initShaders before entering the game loop

        // This vector stores pointers to all of the entities
        // Vectors don't support normal pointers, like Entity *, so we use unique_ptr instead
        std::vector<std::unique_ptr<Entity>> entities;
    };
}

