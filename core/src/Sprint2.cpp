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
        /*
        for (auto s : spriteList)
        {
            game->createSprite(s.substr(0, s.find_first_of('.')), s);
            break;
        }
        for (auto e : game->getPageList().at(0)->getEntityList())
        {
            e->setSpriteID(0);
        }
        */
        //game->createSprite("1", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\test_image_1.png", 1);
        //game->createSprite("2", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\test_image_2.png", 2);
        //game->createSprite("3", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\test_image_3.png", 3);
        //game->createSprite("guy", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\oh_yeah_woo_yeah.png", 4);
        int ss = game->createSpriteSheet("spritesheet", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\spritesheet.png");
        //int ss = game->createSpriteSheet("spritesheet", "C:\\Users\\joshu\\Desktop\\Parchment\\CS307team12\\resource\\Ocean_SpriteSheet.png");
        //game->createPartialSprite("guy", 1, ss, glm::ivec2(17, 3), glm::ivec2(13, 22));
        
        // name
        // requested spriteID
        // spritesheet that the loop exists on
        // number of images in the loop (4)
        // the speed of the loop in seconds
        // bottom left corner of the loop, in pixels
        // width and height of each sprite in the loop
        // the x padding between each sprite in the spritesheet
        game->createLoopingSprite("guy", 1, ss, 4, 1.0, glm::ivec2(1, 4), glm::ivec2(13, 21), 3);
        //game->createLoopingSprite("guy", 1, ss, 9, 1.0, glm::ivec2(0, 0), glm::ivec2(32, 32), 0);

        game->mainLoop();
        game->destroy();
        return game;
    }
}