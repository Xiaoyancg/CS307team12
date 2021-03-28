#include "GameVM.h"
#include "Game.h"
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
    g.run();
    return 0;
}
