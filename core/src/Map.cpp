#include "Map.h"

namespace Core
{
    extern int width;
    extern int height;

    // Takes a scale, the number of tiles in the x and y direction 
    Map::Map ( glm::vec2 dimensions, int tileSize ):
        mMapDimensions ( dimensions ),
        mTileSize ( tileSize )
    {
// Create map 
        mNumTiles = mMapDimensions.x * mMapDimensions.y;
        mTileArray = new Tile[mNumTiles];
        setTileCoords ();
    }

    // Set mMapDimensions to new dimensions
    void Map::setDimensions ( glm::vec2 dimensions )
    {
// setDimensions should copy the map to the new map! right now it just discards the old one
        delete mTileArray;
        mNumTiles = mMapDimensions.x * mMapDimensions.y;
        Tile *newMap = new Tile[mNumTiles];
        mTileArray = newMap;
        mMapDimensions = dimensions;
        setTileCoords ();
    }

    // Get mMapDimensions
    glm::vec2 Map::getDimensions ()
    {
        return mMapDimensions;
    }

    void Map::setTileSize ( int size )
    {
        mTileSize = size;
    }

    int Map::getTileSize ()
    {
        return mTileSize;
    }

    int Map::getNumTiles ()
    {
        return mNumTiles;
    }

    // This will set the 4 corners of each tile of the map based on the dimensions and tilesize.
    // Having tile coordinates pre-set allows for quick map rendering, with minimal calculations.
    // These coordinates do not change often, only in the case of changing dimensions or tile size.
    void Map::setTileCoords ()
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
        int lowX = ( width / 2 ) - halfXPixels;
        int highX = lowX + mTileSize;
        int highY = ( height / 2 ) + halfYPixels;
        int lowY = highY - mTileSize;


        int border = 1; // The amount of space between tiles as they're drawn on the map (used for debugging right now)
        int index = 0; // An easy way to access the mTileArray without having to do any calculations
        for ( int row = mMapDimensions.y - 1; row >= 0; row-- )
        {
            for ( int col = 0; col < mMapDimensions.x; col++ )
            {
                int coords[8];

                // P1
                coords[0] = lowX; // Top left x
                coords[1] = highY; // Top left y

                // P2
                coords[2] = lowX; // Bottom left x
                coords[3] = lowY; // Bottom left y
                // P3
                coords[4] = highX; // Top right x
                coords[5] = highY; // Top right y
                // P4
                coords[6] = highX; // Bottom right x
                coords[7] = lowY; // Bottom right y


                // DRAW BORDER LINES ALONG MAP (this is just for testing bc all tiles are plain white right now)
                // P1
                coords[0] += border;
                coords[1] -= border;

                // P2
                coords[2] += border;
                coords[3] += border;
                // P3
                coords[4] -= border;
                coords[5] -= border;
                // P4
                coords[6] -= border;
                coords[7] += border;


                mTileArray[index].setCoords ( coords );

                lowX += mTileSize;
                highX += mTileSize;
                index++;
            }
            lowX = ( width / 2 ) - halfXPixels;;
            highX = lowX + mTileSize;
            highY -= mTileSize;
            lowY -= mTileSize;
        }
    }

    // Takes a pointer to an array of integers, containing the spriteID for each tile in the Map
    // ASSUMES THE DIMENSIONS OF ARRAY spriteIDMap ARE THE SAME AS Map::mMapDimensions
// This will have to take a depth parameter when Tile depth gets implemented
    void Map::setMapTileSpritesFromArray ( int *spriteIDMap )
    {
        mNumTiles = mMapDimensions.x * mMapDimensions.y; // Get the number of tiles in the map

        for ( int i = 0; i < mNumTiles; i++ )
        {
            mTileArray[i].setSpriteID ( spriteIDMap[i] ); // Copy the spriteID to the Map at tile 'i'
        }
    }
}