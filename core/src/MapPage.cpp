#include "MapPage.h"
#include "glm/glm.hpp"


namespace Core
{  
    // Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
    void MapPage::setMap ( Map *map )
    {
        mMap = map;
    }

    void MapPage::render ()
    {
        // Traverse all tiles in the Map
        for ( int i = 0; i < mMap->getNumTiles (); i++ )
        {
            // Render each tile of the map!
            int *coords = mMap->mTileArray[i].getCoords (); // Get ptr to the tile coordinates

            // Buffer and draw tile
            // NOTE: Change the int multiplier whenever new data will be added to the shaders.
            //       Right now, there are 4 points (8 ints), with 4 texture points (8 ints) = 16 * sizeof(int)
            glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(int), coords, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        Page::render();
    }
}