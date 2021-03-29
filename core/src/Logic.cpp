#include "Logic.h"
#include "Game.h"
namespace Core
{
    extern struct GameState gstate;

    void Logic::check(SignalVariant &signal)
    {
        switch (msignalType)
        {
        case SignalType::key:
            if (std::get<KeyLogic>(mlogic).check(std::get<KeySignal>(signal)))
            {
                gstate.scriptList->push_back(this);
            }
            break;

        default:
            break;
        }
    }

    // check if key signal meet the criteria
    bool KeyLogic::check(Core::KeySignal signal)
    {
        return (signal.getKey() == mkey && signal.getType() == mtype);
    }
}