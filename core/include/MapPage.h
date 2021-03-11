#include "Page.h"
#include "Map.h"


namespace Core
{
// MapPage class
    class MapPage: public Page
    {
    public:
        // Needs the current window to make a new opengl context, in the Page constructor 
        MapPage ( SDL_Window *window );

        // Users can specify a map if it's already created
        MapPage ( SDL_Window *window, Map *map );

        // Sets mMap to the map created by the user
        void setMap ( Map *map );

        void initShaders (); // Shader function for the map tiles, may be temporary

        // Render the current map!
        void render ();

    private:
        // The map! only one for now, I plan to add code to support 1 MapPage for an arbitrary amount of maps with std::vector and unique_ptrs
        Map *mMap;

        int mCurrentMap; // Unimplemented, but will be used when multiple-map MapPages get added
    };
}