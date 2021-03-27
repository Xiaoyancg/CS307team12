#include "Signal.h"
namespace Core
{

    SignalEvent &Core::Signal::getSignal()
    {
        switch (this->mtype)
        {
        case SignalType::key:
            return (SignalEvent)mks;
            break;

        default:
            break;
        }
    }

}