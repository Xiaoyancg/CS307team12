#pragma once
#include "Page.h"
#include "Map.h"
#include <string>

namespace Core
{
// MapPage class
    class MapPage: public Page
    {
    public:
        
        // Needs the current window to make a new opengl context, in the Page constructor 
        MapPage::MapPage () : 
            MapPage ( "No MapPage name :(", new Map("No Map name :(", glm::vec2(0, 0), 0) )
        { }

        MapPage::MapPage (std::string s): 
            MapPage (s, new Map("No Map name :(", glm::vec2(0, 0), 0))
        { }
        
        // Users can specify a map if it's already created
            // Users can specify a map if it's already created
        MapPage::MapPage(std::string s, Map* map) :
            Page(s), mMap(nullptr)
        {
            setMap(map);
            SetBackgroundColor(0.1f, 0.9f, 0.59f, 1.0f);
        }


        // Sets mMap to the map created by the user
        void setMap ( Map *map );

        // Returns mMap
        Map* getMap();

        // Render the current map!
        void render ();



        // AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
        // This is set by Game.cpp when a Game object is created
        // Any class that renders sprites needs a way to translate between Parchment Sprite IDs and imported OpenGL IDs.
        // The only other solution I could think of was to make mGameSprites in Game.cpp a global variable, 
        // but that sounds like a disgusting solution and im not about to break encapsulation.
        // If anyone has a better idea, feel free to implement it. Otherwise, this should be fine. 
        static inline SpriteManager* mGameSprites;

    private:
        // The map!
        Map *mMap;
    };
}