#pragma once
#include <glm/glm.hpp>
#include <string>
#include <nlohmann/json.hpp>
#include "Tile.h"
#include "Camera.h"

// Map class
namespace Core
{
	class Map
	{
	public:
		// Takes a scale, the number of tiles in the x and y direction
		Map(std::string name = "", glm::ivec2 dimensions = glm::ivec2(0, 0), int tileSize = 0);
		~Map();

		void setMapTileSpritesFromArray(std::vector<int>& spriteIDMap);

		// Set mMapDimensions
		void setDimensions(glm::ivec2 dimensions);

		// Get mMapDimensions
		glm::ivec2 getDimensions();

		// Set mTileSize
		void setTileSize(int size);

		// Get mTileSize
		int getTileSize();

		void setName(std::string);
		std::string getName();

		void render(bool withBorder);

		Tile* checkTileCollision(glm::vec2);

        Tile* getTileFromClick(Camera* camera, float x, float y);

		static Map* fromJSON(nlohmann::json& root);
		void parse(nlohmann::json& root);
		nlohmann::json serialize();

	private:
		// Set tile coordinates
		// (so we don't have to recalculate with each update)
		void updateTileCoords();

		// For now, the map is an array of tiles with no depth
		std::vector<Tile> mTileVector;

		// Size of the tile in pixels
		// (only one dimension because it's a square, x = y)
		int mTileSize;

		// The dimensions of the map in tiles
		// (32, 32) means the map is 32 tiles by 32 tiles
		glm::ivec2 mMapDimensions;

		std::string mMapName;
	};
}