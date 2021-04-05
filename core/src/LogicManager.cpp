#pragma once
#include "LogicManager.h"
namespace Core
{
    LogicManager LogicManager::parse(nlohmann::json root)
    {
        auto signalVector = root.at("signalList").get<std::vector<nlohmann::json>>();
        for (auto signal_js : signalVector)
        {
            _signals.push_back(Signal::parse(signal_js));
        }
        auto logicVector = root.at("logicList").get<std::vector<nlohmann::json>>();
        for (auto logic_js : logicVector)
        {
        }
        auto scriptVector = root.at("scriptList").get<std::vector<nlohmann::json>>();
        for (auto script_js : scriptVector)
        {
        }
    }

    Signal *LogicManager::createSignal()
    {
        _signals.push_back(Signal());
        return &(_signals.at(_signals.size() - 1));
    }

    ////Signal *LogicManager::createSignal(int id, SignalType type, SignalUnion signal)
    ////{
    ////    Signal s(id, type);

    ////    switch (type)
    ////    {
    ////    case SignalType::Key:
    ////        s.keySignal = KeySignal();
    ////        break;
    ////    case SignalType::Custom:
    ////        s.customSignal = CustomSignal();
    ////        break;

    ////    default:
    ////        break;
    ////    }
    ////    return &s;
    ////}

}