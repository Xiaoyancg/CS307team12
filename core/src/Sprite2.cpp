#include "Sprint2.h"
#include "VMTool.h"

namespace Core
{
    Game *s2Game()
    {
        Game *game = s1Game();
        game->setGameName("s2Game");
        // FIXME: use relative directory. also remove from here
        std::vector<std::string> spriteList =
            searchAllFileWithExtension("./", ".png");
        for (auto k : spriteList)
        {
            game->createSprite(k.substr(0, k.find_first_of('.')), k);
        }
        //game->createSprite("1", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\core\\res\\test_image_1.png", 1);
        //game->createSprite("2", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\core\\res\\test_image_2.png", 2);
        //game->createSprite("3", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\core\\res\\test_image_3.png", 3);
        //game->createSprite("guy", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\core\\res\\oh_yeah_woo_yeah.png", 4);
        return game;
    }
}