#include "Signal.h"
namespace Core
{

    SignalVariant &Core::Signal::getSignal()
    {
        switch (this->mtype)
        {
        case EventType::key:
            return (SignalVariant)mks;
            break;

        default:
            break;
        }
    }

}