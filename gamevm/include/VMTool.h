#pragma once
#include <string>
#include <Game.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// search gdata file in a given location
// return the first met file name
std::string searchGdata(std::string location);

std::string ReadFile(std::string fileName);

int WriteFile(std::string fileName, std::string content);

Core::Game *ConstructGame(std::string fileName);

json *readGameDataFile(std::string f);
int ProduceDataFile(Core::Game *game);

void removeGame(std::string);

Core::Game *CreateExampleGame();

//int WriteJson ( std::string fileName, json j );
