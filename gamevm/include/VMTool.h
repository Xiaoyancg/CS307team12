#pragma once
#include <string>
#include <Game.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string ReadFile ( std::string fileName );


int WriteFile ( std::string fileName, std::string content );

Game constructGame ( std::string fileName );



