#include "LogicManager.h"
namespace Core
{
    void LogicManager::sendSignal(Signal signal)
    {
        msignals.push_back(signal);
    }
    void LogicManager::check()
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