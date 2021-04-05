#pragma once
#include "mySignal.h"

namespace Core
{

    //* ----------------------- CUSTOM ----------------------- *//

    std::vector<int> SignalCustom::getTargetScriptList() { return _targetScriptList; }
    void SignalCustom::setTargetScriptList(std::vector<int> targetScriptList)
    {
        _targetScriptList = targetScriptList;
    }
    SignalCustom::SignalCustom() : SignalCustom(std::vector<int>()) {}
    SignalCustom::SignalCustom(std::vector<int> targetScriptList)
        : _targetScriptList(targetScriptList) {}
    SignalCustom::~SignalCustom() {}
    SignalCustom SignalCustom::parse(nlohmann::json root)
    {
        return SignalCustom(root.at("targetScriptList").get<std::vector<int>>());
    }

    //* ------------------------- KEY ------------------------ *//

    SignalKey::SignalKey() {}
    SignalKey::SignalKey(SDL_KeyboardEvent event)
        : _key(event.keysym.sym), _keyType(event.type) {}
    SignalKey::~SignalKey() {}

    //* -------------------- SIGNAL UNION -------------------- *//

    SignalUnion::SignalUnion(const SignalUnion &) = default;
    SignalUnion::SignalUnion() : customSignal(SignalCustom()) {}
    SignalUnion::SignalUnion(SignalKey keySignal) : keySignal(keySignal) {}
    SignalUnion::SignalUnion(SignalCustom customSignal) : customSignal(customSignal) {}
    SignalUnion::~SignalUnion() {}

    //* ----------------------- SIGNAL ----------------------- *//

    Signal::Signal() : Signal(-1, SignalType::Custom, SignalUnion()) {}
    Signal::Signal(const Signal &) = default;

    Signal::Signal(int signalId, SignalType signalType, SignalUnion signal)
        : _signalId(signalId), _signalType(signalType), _signal(signal) {}

    Signal::~Signal() {}

    Signal Signal::parse(nlohmann::json root)
    {
        int id = root.at("signalId").get<int>();
        SignalType type = static_cast<SignalType>(
            root.at("signalType").get<int>());
        SignalUnion signal_u;
        switch (type)
        {
        case SignalType::Custom:
            signal_u.customSignal = SignalCustom::parse(root);
            break;

        default:
            break;
        }
        return Signal(id, type, signal_u);
    }
}