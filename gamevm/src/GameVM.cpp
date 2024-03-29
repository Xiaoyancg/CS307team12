#include <iostream>
#include "GameVM.h"
#include "VMTool.h"
#include "Game.h"
#include "Timer.h"


int GameVMMain()
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
    g.run();
    return 0;
}
