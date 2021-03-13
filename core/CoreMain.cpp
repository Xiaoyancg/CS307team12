#include <Game.h>
#include <Sprint1.h>
using namespace Core;

int main ( int argc, char *argv[] )
{
    Game *game = s1Game ();
    game->run ();

    return 0;
}