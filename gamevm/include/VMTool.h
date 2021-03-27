#pragma once
#include <string>
#include <Game.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::vector<std::string> searchAllFileWithExtension(std::string location, std::string extension);
// search  file in a given location
// return the first met file name
std::string searchFileWithExtension(std::string location, std::string extension);

std::string ReadFile(std::string fileName);

int WriteFile(std::string fileName, std::string content);

Core::Game *ConstructGame(std::string fileName);

json *readGameDataFile(std::string f);
int ProduceDataFile(Core::Game *game);

void removeGame(std::string);

Core::Game *CreateExampleGame();

//int WriteJson ( std::string fileName, json j );
