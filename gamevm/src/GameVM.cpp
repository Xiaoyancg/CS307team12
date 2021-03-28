#include <iostream>
#include "GameVM.h"
#include "VMTool.h"
#include "Game.h"
<<<<<<< HEAD
#include "Timer.h"
volatile unsigned sink = 0;
int GameVMMain(int argc, char *argv[])
{

    std::string gdatastring = ReadFile(searchFileWithExtension("./", ".gdata"));
    if (gdatastring.size() == 0)
    {
        // FIXME: should use windows warning window
        std::cout << "no game found" << std::endl;
        return 1;
    }

    // can't use constructor, use json::parse
    nlohmann::json j = nlohmann::json::parse(gdatastring);
    Core::Game g(j);
=======
#include "VMTool.h"

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

using namespace std;
using nlohmann::json;


int GameVMMain(int argc, char *argv[])
{
    string path;
    for (auto& p : filesystem::directory_iterator(".")) {
        if (p.path().extension() == "gdata") {
            path = p.path().string();
            break;
        }
    }
    if (path.empty()) {
        return 1;
    }
    json* gameData = readGameDataFile(path);
    Core::Game g(*gameData);
>>>>>>> 8adc54f046be9fdc06e2ed0777dba6a5ca6747bb
    g.run();
    return 0;
}
