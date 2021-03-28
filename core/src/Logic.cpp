#include "Logic.h"
namespace Core
{

    int Logic::check(SignalVariant signal)
    {

        switch (msignalType)
        {
        case SignalType::key:
            if (std::get<KeyLogic>(logic).check(std::get<KeySignal>(signal)))
            {
                readyLogicList->push_back(this);
            }
            break;

        default:
            break;
        }
        return 0;
    }
    bool KeyLogic::check(Core::KeySignal signal)
    {
        return (signal.getKey() == mkey && signal.getType() == mtype);
    }
}