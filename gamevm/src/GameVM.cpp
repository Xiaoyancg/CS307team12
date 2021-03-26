#include <iostream>
#include "GameVM.h"
#include "VMTool.h"
#include "Game.h"
#include "Timer.h"
volatile unsigned sink = 0;
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

    // can't use constructor, use json::parse
    nlohmann::json j = nlohmann::json::parse(gdatastring);
    Core::Game g(j);
    return 0;
}
