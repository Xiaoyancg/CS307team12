#pragma once
#include "Page.h"
#include "Map.h"
#include<string>

namespace Core
{
// MapPage class
    class MapPage: public Page
    {
    public:
        // Needs the current window to make a new opengl context, in the Page constructor 
        MapPage::MapPage () : 
            MapPage ( "empty", new Map ( glm::vec2 ( 0, 0 ), 0 ) )
        { }

        MapPage::MapPage (std::string s): 
            MapPage (s, new Map ( glm::vec2 ( 0, 0 ), 0 ) )
        { }

        // Users can specify a map if it's already created
        MapPage::MapPage ( std::string s, Map *map ) : 
            Page (s), mMap ( map )
        {
            SetBackgroundColor ( 0.1f, 0.9f, 0.59f, 1.0f );
        }

        // Sets mMap to the map created by the user
        void setMap ( Map *map );

        void initShaders (); // Shader function for the map tiles, may be temporary

        // Render the current map!
        void render ();

    private:
        // The map! only one for now, I plan to add code to support 1 MapPage for an arbitrary amount of maps with std::vector and unique_ptrs
        Map *mMap;
    };
}