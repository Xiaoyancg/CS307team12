#pragma once
#include <SDL.h>
#include <Page.h>
#include <glad/glad.h>
#include <stdio.h>
#include <ctime>

namespace Core
{
    class Game
    {
    public:
        Game ();
        Game ( std::string gameName );

        std::string GetGameName ( void );
        int SetGameName ( std::string newName );

        std::string GetAuthor ( void );
        int SetAuthor ( std::string newAuthor );

        std::string GetVersion ( void );
        int SetVersion ( std::string newVersion );

        std::string GetLMTime ( void );
        int SetLMTime ( std::string time );

        std::string GetNote ( void );
        int SetNote ( std::string newNote );
        int AddNote ( std::string moreNote );

        // create a page in the page list
        // return the page id
        int CreatePage ( void );
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

    };
}

void sdl_die ( const char *err_msg );
void initShaders ();

void init ();

int render ();

int coreMain ( int argc, char *argv[] );



