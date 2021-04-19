#include "MapPage.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <iostream>


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
    Tile* MapPage::getTileFromClick(int x, int y) {
        float dimx = ((mMap->getDimensions().x + 1) * mMap->getTileSize()) / 2.0f;
        float dimy = ((mMap->getDimensions().y + 1) * mMap->getTileSize()) / 2.0f;

        // Get click as ortho matrix
        glm::vec4 ret = glm::ortho(-dimx, dimx, -dimy, dimy) * glm::vec4(x, y, 0.0f, 1.0f);

        // Scale click's ortho projection to the Camera's projection
        ret = glm::inverse(mMap->getCamera()->getOrtho()) * ret;

        // Reverse the movement of the camera to get the click location on the centered map
        ret = glm::inverse(mMap->getCamera()->getTranslate()) * ret;

        // ret.xy now contains the x and y that can be compared against objects in the Core
        
        glm::ivec2 c(ret.x, ret.y);
        return mMap->checkTileCollision(c); // Returns either a ptr to a Tile or nullptr
    }

    void MapPage::parse(nlohmann::json& root) {
        Page::parse(root);
    }

    nlohmann::json MapPage::serialize() {
        nlohmann::json& root = Page::serialize();
        root["type"] = "map";
        return root;
    }
}