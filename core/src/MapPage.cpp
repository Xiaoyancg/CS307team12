#include "MapPage.h"
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "Game.h"


namespace Core
{

    // Sets mMap to the given map ('map' argument should be created with 'new Map' on the heap)
    void MapPage::setMapIndex(int idx)
    {
        mMapIndex = idx;
    }

    int MapPage::getMapIndex() {
        return mMapIndex;
    }

    Map* MapPage::getMap() {
        return mGame->getMap(mMapIndex);
    }

    Camera* MapPage::getCamera() {
        return mCamera;
    }

    void MapPage::render()
    {
        if (mMapIndex != -1) {
            mGame->getMap(mMapIndex)->render(0);
        }

        Page::render();
    }

    void MapPage::parse(nlohmann::json& root) {
        mMapIndex = root["map"].get<int>();
        Page::parse(root);
    }

    nlohmann::json MapPage::serialize() {
        nlohmann::json& root = Page::serialize();
        root["type"] = "map";
        root["map"] = mMapIndex;
        return root;
    }
}