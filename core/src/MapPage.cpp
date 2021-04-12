#include "MapPage.h"
#include "glm/glm.hpp"


namespace Core
{

    // Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
    Map* MapPage::addMap ( Map *map )
    {
        // Set the old map to have no associated page
        if (mMap) {
            mMap->mAssociatedPage = nullptr;
        }

        mMaps.push_back(map);
        // Set the new map
        mMap = map;

        // Set the new map's associated page
        if (mMap != nullptr) {
            mMap->mAssociatedPage = this;
        }

        return mMap;
    }

    Map* MapPage::getCurrMap() {
        return mMap;
    }

    void MapPage::deleteCurrMap() {
        auto it = std::find(mMaps.begin(), mMaps.end(), mMap);
        if (it != mMaps.end()) { 
            mMaps.erase(it);
        }

        delete mMap;
        mMap = nullptr;
    }

    std::vector<Map*> MapPage::getMaps() {
        return mMaps;
    }

    Camera* MapPage::getCurrCamera() {
        return mMap->getCamera();
    }

    void MapPage::render()
    {
        if (mMap != nullptr) {
            mMap->render(0);
        }

        Page::render();
    }

    // This is called from the MapView window with on a mouse click.
    // x and y are floats on a scale from 0 to 1, indicating the location of the click.
    // The coordinate is a float between 0 and 1 because the MapView window can be stretched, 
    // and we need a predictable scale (0->1 is simple)
    Tile* MapPage::getTileFromClick(float x, float y) {
        glm::ivec2 click(x * 1280, y * 720); // Scale click to pixel coordinates
        return mMap->checkTileCollision(click); // Returns either a ptr to a Tile or nullptr
    }
}