#include "Logic.h"
namespace Core
{
    bool KeyLogic::check(Signal &signal_ref)
    {

        return (std::get<KeySignal>(signal_ref).getKey() == mkey &&
                std::get<KeySignal>(signal_ref).getKeyType() == mkeyType);
    }
}