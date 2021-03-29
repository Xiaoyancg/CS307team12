#include "Logic.h"
namespace Core
{

    void Logic::check(SignalVariant &signal)
    {
        switch (msignalType)
        {
        case SignalType::key:
            if (std::get<KeyLogic>(mlogic).check(std::get<KeySignal>(signal)))
            {
                gscriptList->push_back(this);
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