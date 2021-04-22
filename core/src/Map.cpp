#include "Map.h"
#include "Game.h"

namespace Core
{
    // Takes a scale, the number of tiles in the x and y direction
    Map::Map(std::string name, glm::ivec2 dimensions, int tileSize)
        : mTileSize(tileSize), mMapName(name)
    {
        // Create map
        setDimensions(dimensions);
    }

    // Set mMapDimensions to new dimensions
    void Map::setDimensions(glm::ivec2 dimensions)
    {
        if (dimensions.x < 0 || dimensions.y < 0)
        {
            // Don't set negative map dimensions >:(
            return;
        }
        mMapDimensions = dimensions;
        mTileVector.resize(mMapDimensions.x * mMapDimensions.y);
        updateTileCoords();
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
        updateTileCoords();
    }

    int Map::getTileSize()
    {
        return mTileSize;
    }

    void Map::setName(std::string name)
    {
        mMapName = name;
    }

    std::string Map::getName()
    {
        return mMapName;
    }

    // This is called from the MapView window with on a mouse click.
    // x and y are floats on a scale from 0 to 1, indicating the location of the click.
    // The coordinate is a float between 0 and 1 because the MapView window can be stretched, 
    // and we need a predictable scale (0->1 is simple)
    Tile* Map::getTileFromClick(Camera* camera, float x, float y) {
        // Click is already in terms of window coordinates, just scale to get world coords
        glm::vec2 transformed = camera->projectToWorld(glm::vec2(x, y));
        return checkTileCollision(transformed); // Returns either a ptr to a Tile or nullptr
    }

    // This will set the 4 corners of each tile of the map based on the dimensions and tilesize.
    // Having tile coordinates pre-set allows for quick map rendering, with minimal calculations.
    // These coordinates do not change often, only in the case of changing dimensions or tile size.
    void Map::updateTileCoords()
    {
        int border = 0; // The amount of space between tiles as they're drawn on the map (used for debugging right now)
        int index = 0;  // An easy way to access the mTileArray without having to do any calculations
        glm::vec2 pos = glm::vec2(0, 0);
        for (int row = mMapDimensions.y - 1; row >= 0; row--)
        {
            for (int col = 0; col < mMapDimensions.x; col++)
            {
                mTileVector[index].calculateSquareCoords(
                    glm::vec2(col * mTileSize, row * mTileSize),
                    glm::vec2(mTileSize, mTileSize)
                );
                index++;
            }
        }
    }

    Tile *Map::checkTileCollision(glm::vec2 point)
    {
        for (auto& tile : mTileVector)
        {
            // Parse the lower and upper coordinates for x and y from the Tile's coords array
            float *coords = tile.getCoords();
            float lowX = coords[0];
            float lowY = coords[1];
            float highX = coords[12];
            float highY = coords[13];

            if ((lowX <= point.x) && (point.x <= highX) &&
                (lowY <= point.y) && (point.y <= highY))
            {
                return &tile;
            }
        }
        return nullptr;
    }

    // Takes a pointer to an array of integers, containing the spriteID for each tile in the Map
    // ASSUMES THE DIMENSIONS OF ARRAY spriteIDMap ARE THE SAME AS Map::mMapDimensions
    // This will have to take a depth parameter when Tile depth gets implemented
    void Map::setMapTileSpritesFromArray(std::vector<int>& spriteIDMap)
    {
        for (int i = 0; i < mTileVector.size(); i++)
        {
            mTileVector[i].setSpriteID(spriteIDMap[i]); // Copy the spriteID to the Map at tile 'i'
        }
    }

    void Map::render(bool withBorder)
    {
        // Traverse all tiles in the Map
        for (auto& tile : mTileVector)
        {
            if (MapPage::mGameSprites->atID(tile.getSpriteID()))
            {
                // Render each tile of the map!
                float *coordsPtr = tile.getCoords(); // Get ptr to the tile coordinates
                float coords[16];
                std::memcpy(coords, coordsPtr, sizeof(int) * 16);

                if (withBorder)
                {
                    int border = 1;
                    // DRAW BORDER LINES ALONG MAP (this is just for testing bc all tiles are plain white right now)
                    // P1
                    coords[0] += border;
                    coords[1] -= border;
                    // P2
                    coords[4] += border;
                    coords[5] += border;
                    // P3
                    coords[8] -= border;
                    coords[9] -= border;
                    // P4
                    coords[12] -= border;
                    coords[13] += border;
                }

                Sprite *sprite = MapPage::mGameSprites->atID(tile.getSpriteID());
                glBindTexture(GL_TEXTURE_2D, sprite->getOpenGLTextureID());
                if (sprite->getType() != SPRITE_TYPES::FULL)
                {
                    // If the sprite is a looping sprite, the current sprite's texture coordinates may need to change, so we update and get them before rendering
                    if (sprite->getType() == SPRITE_TYPES::LOOPING)
                    {
                        ((LoopingSprite *)sprite)->updateTextureCoords();
                    }
                    float *texcoords = sprite->getTextureCoordinates();
                    // P1 texture coords
                    coords[2] = texcoords[0];
                    coords[3] = texcoords[1];
                    // P2 texture coords
                    coords[6] = texcoords[2];
                    coords[7] = texcoords[3];
                    // P3 texture coords
                    coords[10] = texcoords[4];
                    coords[11] = texcoords[5];
                    // P4 texture coords
                    coords[14] = texcoords[6];
                    coords[15] = texcoords[7];
                }

                // Buffer and draw tile
                // NOTE: Change the int multiplier whenever new data will be added to the shaders.
                //       Right now, there are 4 points (8 ints), with 4 texture points (8 ints) = 16 * sizeof(int)
                glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), coords, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Re-enable drawing (whether made invisible or not)

                // Unbind the current sprite
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
    }

    void Map::renderCollisionHelper(GLuint texture) {
        glBindTexture(GL_TEXTURE_2D, texture);

        for (auto& tile : mTileVector)
        {
            if (tile.isSolid()) {
                // Render each tile of the map!
                float *coords = tile.getCoords(); // Get ptr to the tile coordinates

                // Buffer and draw tile
                // NOTE: Change the int multiplier whenever new data will be added to the shaders.
                //       Right now, there are 4 points (8 ints), with 4 texture points (8 ints) = 16 * sizeof(int)
                glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), coords, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); // Re-enable drawing (whether made invisible or not)
            }
        }

        // Unbind the current sprite
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    using json = nlohmann::json;
    Map* Map::fromJSON(json& root) {
        Map* map = new Map;
        map->parse(root);
        return map;
    }

    void Map::parse(json& root) {
        auto size = root.at("size").get<std::vector<int>>();
        mMapDimensions = glm::ivec2(size[0], size[1]);
        auto& tileArray = root.at("tiles").get<std::vector<std::vector<json>>>();
        for (auto& tile : tileArray) {
            mTileVector.emplace_back(tile[0].get<int>(), tile[1].get<bool>());
        }
    }

    json Map::serialize() {
        json map;
        map["size"] = { mMapDimensions.x, mMapDimensions.y };
        std::vector<json> jTileList;
        for (auto& tile : mTileVector) {
            json jTile = { tile.getSpriteID(), tile.isSolid() };
            jTileList.push_back(jTile);
        }
        map["tiles"] = jTileList;
        return map;
    }
}
