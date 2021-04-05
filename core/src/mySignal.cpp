#pragma once
#include "mySignal.h"

namespace Core
{
    SignalCustom SignalCustom::parse(nlohmann::json root)
    {
        int targetScript = root.at("targetScript").get<int>();
    }
    Signal Signal::parse(nlohmann::json root)
    {
        int id = root.at("signalId").get<int>();
        SignalType type = static_cast<SignalType>(
            root.at("signalType").get<int>());
        SignalUnion signal_u;
        switch (type)
        {
        case SignalType::Custom:
            signal_u.customSignal =
                break;

        default:
            break;
        }
        return Signal(id, type, signal_u);
    }
}