#include "MapPage.h"
#include "glm/glm.hpp"


namespace Core
{  
    // Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
    void MapPage::setMap ( Map *map )
    {
        // Set the old map to have no associated page
        if (mMap) {
            mMap->mAssociatedPage = nullptr;
        }

        // Set the new map
        mMap = map;

        // Set the new map's associated page
        if (mMap != nullptr) {
            mMap->mAssociatedPage = this;
        }
    }

    Map* MapPage::getMap() {
        return mMap;
    }

    void MapPage::render ()
    {
        if (mMap != nullptr) {
            mMap->render();
        }

        Page::render();
    }
}