#pragma once
#include <string>
#include <Game.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string ReadFile ( std::string fileName );


int WriteFile ( std::string fileName, std::string content );

Game ConstructGame ( std::string fileName );


int ProduceDataFile ( Game *game );

//int WriteJson ( std::string fileName, json j );
