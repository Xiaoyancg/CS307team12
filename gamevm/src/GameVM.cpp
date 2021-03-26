#include "GameVM.h"
#include "VMTool.h"
#include <iostream>
#include "Game.h"

int GameVMMain(int argc, char *argv[])
{
    std::string gdatastring = ReadFile(searchGdata("./"));
    if (gdatastring.size() == 0)
    {
        // FIXME: should use windows warning window
        std::cout << "no game found" << std::endl;
        return 1;
    }
    std::cout << gdatastring << std::endl;

    nlohmann::json json(gdatastring);
    Core::Game g(json);
    return 0;
}
