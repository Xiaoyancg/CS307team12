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

        void initShaders (); // Shader function for the map tiles, may be temporary

        // Render the current map!
        void render ();

    private:
        // The map!
        Map *mMap;
    };
}