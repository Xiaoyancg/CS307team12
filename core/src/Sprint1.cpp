#include "Sprint1.h"
namespace Core
{
    Game *s1Game ()
    {
        Game *game = new Game ();
        Entity *entityInteractive;
        Entity *entityTallThin;
        Entity *entityShortWide;
        Entity *entityVeryShortWide;
        Entity *entityOrigin;
        Page *entityPage;
        MapPage *mapPage1;
        MapPage *mapPage2;


        ///////////////
        // ENTITY TEST (This is here just for demo purposes)
        // This will be moved within an actual page once sprites are implemented.
        // For now it's separated from a page because all rendered objects are a single color and there
        // would be no way to see a difference from a Tile on the map and an Entity

        entityPage = game->createPage ( "entityPage" );
        entityPage->SetBackgroundColor ( 0.1, 0.2, 0.59, 1.0 );

        entityInteractive = 
            entityPage->createEntity ( "interactive", glm::vec2 ( 50, 50 ),
                                                       glm::vec2 ( 64, 64 ), 0, 0 );
        entityPage->setCtrlEntity ( entityInteractive );
        entityTallThin = 
            entityPage->createEntity ( "interactive", glm::vec2 ( 200, 200 ),
                                                    glm::vec2 ( 32, 64 ), 0, 0 );
        entityShortWide = 
            entityPage->createEntity ( "shortWide", glm::vec2 ( 500, 500 ),
                                                     glm::vec2 ( 64, 32 ), 0, 0 );
        entityVeryShortWide = 
            entityPage->createEntity ( "veryShortWide", glm::vec2 ( 640, 100 ),
                                                         glm::vec2 ( 128, 16 ), 0, 0 );
        entityOrigin = 
            entityPage->createEntity ( "origin", glm::vec2 ( 1000, 300 ),
                                                  glm::vec2 ( 128, 128 ), 0, 0 );
        ///////////////

        ///////////////
        // MAP TEST
        int spriteMap[] = {
              1,  2,  3,  4,
              5,  6,  7,  8,
             11, 22, 33, 44,
             55, 66, 77, 88
        };

        Map *map1 = new Map ( glm::vec2 ( 4, 4 ), 64 );
        map1->setMapTileSpritesFromArray ( spriteMap );
        Map *map2 = new Map ( glm::vec2 ( 16, 16 ), 32 );

        // Here are the 2 ways to make MapPages with set maps
        mapPage1 = game->createMapPage ( "1", map1 );
        mapPage2 = game->createMapPage ( "2" );
        mapPage2->setMap ( map2 ); // Sets empty map page 2's map

        // very important
        game->setCurrentPage ( entityPage );
        return game;
    }
}
