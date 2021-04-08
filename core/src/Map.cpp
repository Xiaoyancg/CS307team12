#pragma once
#include <string>
#include "Map.h"
#include "Game.h"

namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Map::Map(std::string name, glm::vec2 dimensions, int tileSize) : mMapDimensions(dimensions),
                                                                     mTileSize(tileSize),
                                                                     mAssociatedPage(nullptr),
                                                                     mMapName(name)
    {
        // Create map
        mNumTiles = mMapDimensions.x * mMapDimensions.y;
        if (mNumTiles > 0)
        {
            mTileArray = new Tile[mNumTiles];
            setTileCoords();
        }
        else
        {
            mTileArray = nullptr;
        }
    }

    Map::~Map()
    {
        if (mTileArray != nullptr)
        {
            delete mTileArray;
            mTileArray = nullptr;
        }

        // If the map is deleted while being used in a MapPage
        if (mAssociatedPage != nullptr)
        {
            mAssociatedPage->addMap(nullptr);
        }
    }

    // Set mMapDimensions to new dimensions
    void Map::setDimensions(glm::vec2 dimensions)
    {
        if (dimensions.x < 0 || dimensions.y < 0)
        {
            printf("Don't set negative map dimensions >:(\n");
            return;
        }

        delete mTileArray;
        mNumTiles = dimensions.x * dimensions.y;
        Tile *newMap = new Tile[mNumTiles];
        mTileArray = newMap;
        mMapDimensions = dimensions;
        setTileCoords();
    }

    // Get mMapDimensions
    glm::ivec2 Map::getDimensions()
    {
        return mMapDimensions;
    }

    void Map::setTileSize(int size)
    {
        if (size < 0)
        {
            return;
        }

        mTileSize = size;
        setTileCoords();
    }

    int Map::getTileSize()
    {
        return mTileSize;
    }

    int Map::getNumTiles()
    {
        return mNumTiles;
    }

    void Map::setName(std::string name)
    {
        mMapName = name;
    }

    std::string Map::getName()
    {
        return mMapName;
    }

    // This will set the 4 corners of each tile of the map based on the dimensions and tilesize.
    // Having tile coordinates pre-set allows for quick map rendering, with minimal calculations.
    // These coordinates do not change often, only in the case of changing dimensions or tile size.
    void Map::setTileCoords()
    {
        // Calculate the smallest and greatest x and y (combinations of these make the 4 corners of the entity)
        // Starting at the top left corner
        // Find the amount of tiles to the left/right and top/bottom (in tiles)
        int halfXTiles = mMapDimensions.x / 2;
        int halfYTiles = mMapDimensions.y / 2;

        // Find the amount of pixels to the left/right and top/bottom from the center of the map to the edge
        int halfXPixels = halfXTiles * mTileSize;
        int halfYPixels = halfYTiles * mTileSize;

        // Calculate the top-left corner of the centered map
        int lowX = (Game::width / 2) - halfXPixels;
        int highX = lowX + mTileSize;
        int highY = (Game::height / 2) + halfYPixels;
        int lowY = highY - mTileSize;

        int border = 1; // The amount of space between tiles as they're drawn on the map (used for debugging right now)
        int index = 0;  // An easy way to access the mTileArray without having to do any calculations
        for (int row = mMapDimensions.y - 1; row >= 0; row--)
        {
            for (int col = 0; col < mMapDimensions.x; col++)
            {
                // posCoords will be filled with the 4 position coordinates for each tile and will be
                // given to Tile::setCoords() where it will be stored in a render-able buffer correctly
                int posCoords[8];

                // P1
                posCoords[0] = lowX;  // Top left x
                posCoords[1] = highY; // Top left y

                // P2
                posCoords[2] = lowX; // Bottom left x
                posCoords[3] = lowY; // Bottom left y
                // P3
                posCoords[4] = highX; // Top right x
                posCoords[5] = highY; // Top right y
                // P4
                posCoords[6] = highX; // Bottom right x
                posCoords[7] = lowY;  // Bottom right y

                // DRAW BORDER LINES ALONG MAP (this is just for testing bc all tiles are plain white right now)
                // P1
                posCoords[0] += border;
                posCoords[1] -= border;

                // P2
                posCoords[2] += border;
                posCoords[3] += border;
                // P3
                posCoords[4] -= border;
                posCoords[5] -= border;
                // P4
                posCoords[6] -= border;
                posCoords[7] += border;

                // Set the coordinates for the current tile
                mTileArray[index].setCoords(posCoords);

                // Increment loop variables to find the next tile in the same row
                lowX += mTileSize;
                highX += mTileSize;
                index++;
            }

            // Calculate the next tile in a new row
            lowX = (Game::width / 2) - halfXPixels;
            ;
            highX = lowX + mTileSize;
            highY -= mTileSize;
            lowY -= mTileSize;
        }
    }


    Tile* Map::checkTileCollision(glm::ivec2 click) {
        for (int i = 0; i < mNumTiles; i++) {
            // Parse the lower and upper coordinates for x and y from the Tile's coords array
            int * coords = mTileArray[i].getCoords();
            int lowX = coords[0];
            int lowY = coords[5];
            int highX = coords[8];
            int highY = coords[1];
            
            if ((lowX < click.x) && (click.x < highX) &&
                (lowY < click.y) && (click.y < highY)) {
                return &mTileArray[i];
            }
        }
        return nullptr;
    }


    // Takes a pointer to an array of integers, containing the spriteID for each tile in the Map
    // ASSUMES THE DIMENSIONS OF ARRAY spriteIDMap ARE THE SAME AS Map::mMapDimensions
    // This will have to take a depth parameter when Tile depth gets implemented
    void Map::setMapTileSpritesFromArray(int *spriteIDMap)
    {
        mNumTiles = mMapDimensions.x * mMapDimensions.y; // Get the number of tiles in the map

        for (int i = 0; i < mNumTiles; i++)
        {
            mTileArray[i].setSpriteID(spriteIDMap[i]); // Copy the spriteID to the Map at tile 'i'
        }
    }

    void Map::render() {
        // Traverse all tiles in the Map
        for (int i = 0; i < getNumTiles(); i++)
        {
            // Render each tile of the map!
            int* coords = mTileArray[i].getCoords(); // Get ptr to the tile coordinates

            // Bind the correct sprite if it exists
            if (mTileArray[i].getSpriteID() != -1 && MapPage::mGameSprites->atID(mTileArray[i].getSpriteID())) {
                glBindTexture(GL_TEXTURE_2D, MapPage::mGameSprites->atID(mTileArray[i].getSpriteID())->getOpenGLTextureID());
            }

            // Buffer and draw tile
            // NOTE: Change the int multiplier whenever new data will be added to the shaders.
            //       Right now, there are 4 points (8 ints), with 4 texture points (8 ints) = 16 * sizeof(int)
            glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(int), coords, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            // Unbind the current sprite
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}
