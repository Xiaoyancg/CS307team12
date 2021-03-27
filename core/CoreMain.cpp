#include <Game.h>
#include <Sprint2.h>
using namespace Core;

int main(int argc, char *argv[])
{
    Game *game = s2Game();
    game->run();

    return 0;
}