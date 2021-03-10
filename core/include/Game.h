#pragma once
#include <SDL.h>
#include <Page.h>
#include <glad/glad.h>
#include <stdio.h>

class Game
{
public:
    Game ();
    
    std::string getName ();
    int setName ();

private:
    std::vector<Page> pageList;
    std::string name;
};


void sdl_die ( const char *err_msg );
void initShaders ();

void init ();

int render ();

int coreMain ( int argc, char *argv[] );



