#include "SignalHandler.h"
#include "Game.h"

namespace Core
{
    void SignalHandler::sendSignal(Signal signal)
    {
        this->msignals.push_back(signal);
    }
    void SignalHandler::check()
    {
        for (auto &&signal : msignals)
        {
            switch (signal.second)
            {
            case SignalType::key:
                for (auto &&Logic : *(keyLogicList))
                {
                    Logic->check(signal.first);
                }

                break;

            default:
                break;
            }
        }
    }
}