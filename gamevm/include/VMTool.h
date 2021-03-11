#pragma once
#include <string>
#include <Game.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string ReadFile ( std::string fileName );


int WriteFile ( std::string fileName, std::string content );

Core::Game * ConstructGame ( std::string fileName );


int ProduceDataFile ( Core::Game *game );

//int WriteJson ( std::string fileName, json j );
