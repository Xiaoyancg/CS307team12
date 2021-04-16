#include "Sprint1.h"

namespace Core
{
    Game *s1Game()
    {
        Game *game = new Game("s1Game");
        game->initContext();
        game->initShader();
        Entity *entityInteractive;
        Entity *entityTallThin;
        Entity *entityShortWide;
        Entity *entityVeryShortWide;
        Entity *entityOrigin;
        Page *entityPage;
        MapPage *mapPage1;
        MapPage *mapPage2;
        MapPage *mapPage3;
        MapPage *mapPage4;

        ///////////////
        // ENTITY TEST (This is here just for demo purposes)
        // This will be moved within an actual page once sprites are implemented.
        // For now it's separated from a page because all rendered objects are a single color and there
        // would be no way to see a difference from a Tile on the map and an Entity

        entityPage = game->createPage("entityPage");
        entityPage->SetBackgroundColor(0.1f, 0.2f, 0.59f, 1.0f);

        entityInteractive =
            entityPage->createEntity("interactive", glm::vec2(50, 50),
                                     glm::vec2(64, 64), 0, 1);
        entityPage->setCtrlEntity(entityInteractive);
        entityShortWide =
            entityPage->createEntity("shortWide", glm::vec2(500, 500),
                                     glm::vec2(64, 32), 0, 0);
        entityVeryShortWide =
            entityPage->createEntity("veryShortWide", glm::vec2(640, 100),
                                     glm::vec2(128, 16), 0, 0);
        entityOrigin =
            entityPage->createEntity("origin", glm::vec2(1000, 300),
                                     glm::vec2(128, 128), 0, 0);

        // MAP TEST
        int spriteMap[] = {
            1, 2, 3, 4,
            4, 3, 2, 1,
            2, 4, 1, 3,
            3, 1, 4, 2};

        Map *map1 = new Map("Map 1 woohoo!", glm::vec2(4, 4), 64);
        map1->setMapTileSpritesFromArray(spriteMap);
        Map *map2 = new Map("Map 2 ahhh!", glm::vec2(16, 16), 32);

        // Here are the 2 ways to make MapPages with set maps
        mapPage1 = game->createMapPage("MapPage 4x4 64px", map1);
        mapPage2 = game->createMapPage("MapPage 16x16 32px");
        mapPage2->addMap(map2); // Sets empty map page 2's map

        // Creating a map with only a name, no dimensions (0 x 0) or tile size
        mapPage3 = game->createMapPage("yee"); // Should be a blank screen

        // Creating a map with no name, dimensions (0 x 0), or tile size
        // mapPage4 = game->createMapPage();

        // // Set the properties of the empty mapPage4
        // Map *map4 = mapPage4->getCurrMap();
        // mapPage4->getCurrMap()->setTileSize(32);
        // map4->setDimensions(glm::vec2(7, 3));

        // MENU PAGE TEST //////////////////////
        Menu *menu = new Menu("example menu");
        // First argument is a name for the menu entry (which gets displayed)
        // Second argument is the size of the text
        // Third and fourth are for callback addresses for 1 or 2 buttons, depending on what is not null
        menu->createMenuEntry("text ahhh", 48, (void *)123, nullptr);
        menu->createMenuEntry("", 62, (void *)123, (void *)123);
        menu->createMenuEntry("oooooo  oo o oooo oo o", 32, (void *)123, (void *)123);
        menu->createMenuEntry("aha", 100, (void *)123, nullptr);
        menu->createMenuEntry("text asdf", 62, 0, (void *)123);
        menu->createMenuEntry("", 62, (void *)123, nullptr);
        menu->createMenuEntry("POG !!!!", 62, (void *)123, nullptr);
        MenuPage *mp = game->createMenuPage("MenuPage name ahahahhsahdfhsadf", menu);
        /////////////////////////////////////////

        // very important
        game->setCurrentPage(entityPage);
        return game;
    }
}
