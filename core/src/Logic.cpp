#include "Logic.h"
namespace Core
{

    // check if signal meet the criteria
    // and add this to ready logic list if it meets

    int Logic::check(SignalVariant signal)
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
        return 0;
    }

    // check if key signal meet the criteria
    bool KeyLogic::check(Core::KeySignal signal)
    {
        return (signal.getKey() == mkey && signal.getType() == mtype);
    }
}