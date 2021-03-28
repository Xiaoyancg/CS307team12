#include "Signal.h"
namespace Core
{

    SignalVariant &Core::Signal::getSignal()
    {
        switch (this->mtype)
        {
        case SignalType::key:
            return (SignalVariant)mks;
            break;

        default:
            break;
        }
    }

}