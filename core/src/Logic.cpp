#include "Logic.h"
#include "Game.h"
namespace Core
{
    bool KeyLogic::check(SignalVariant &signal_ref)
    {

        return (std::get<KeySignal>(signal_ref).getKey() == mkey &&
                std::get<KeySignal>(signal_ref).getKeyType() == mkeyType);
    }
}