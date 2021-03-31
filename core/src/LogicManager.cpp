#include "LogicManager.h"
namespace Core
{
    int LogicManager::sendSignal(Signal signal)
    {
        return 1;
    }
    void LogicManager::check(Signal &signal)
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