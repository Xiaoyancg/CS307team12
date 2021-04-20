#pragma once
#include "Page.h"
#include "Map.h"
#include "Camera.h"
#include <string>

namespace Core
{
// MapPage class
    class MapPage: public Page
    {
    public:
        // Users can specify a map if it's already created
        MapPage::MapPage(const std::string s = "New Map Page", int mapIndex = -1)
            : Page(s), mMapIndex(mapIndex)
        {
            SetBackgroundColor(0.1f, 0.9f, 0.59f, 1.0f);
            mCamera = new Camera;
        }


        // Sets mMap to the map created by the user
        void setMapIndex(int index);
        int getMapIndex();
        // Returns the map
        Map* getMap();

        // Returns the camera
        Camera* getCamera();

        // Render the current map!
        void render();
        
        void parse(nlohmann::json& root);
        nlohmann::json serialize();

        // AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
        // This is set by Game.cpp when a Game object is created
        // Any class that renders sprites needs a way to translate between Parchment Sprite IDs and imported OpenGL IDs.
        // The only other solution I could think of was to make mGameSprites in Game.cpp a global variable, 
        // but that sounds like a disgusting solution and im not about to break encapsulation.
        // If anyone has a better idea, feel free to implement it. Otherwise, this should be fine. 
        static inline SpriteManager* mGameSprites;

    private:
        Camera* mCamera = nullptr;
        int mMapIndex = -1;
    };
}