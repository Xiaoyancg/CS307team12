#pragma once
#include <cstring>
#include "mySignal.h"
#include <cstdarg>
namespace Core
{

    //* ----------------------- CUSTOM ----------------------- *//

    std::vector<int> SignalCustom::getTargetLogicList() { return _targetLogicList; }
    void SignalCustom::setTargetLogicList(std::vector<int> targetLogicList)
    {
        _targetLogicList = targetLogicList;
    }
    SignalCustom::SignalCustom() : SignalCustom(std::vector<int>()) {}
    SignalCustom::SignalCustom(std::vector<int> targetLogicList)
        : _targetLogicList(targetLogicList) {}

    SignalCustom::~SignalCustom() {}
    SignalCustom SignalCustom::parse(nlohmann::json root)
    {
        return SignalCustom(root.at("targetScriptList").get<std::vector<int>>());
    }

    //* ------------------------- KEY ------------------------ *//

    SDL_Keycode SignalKey::getKey() { return _key; }
    Uint32 SignalKey::getKeyType() { return _keyType; }
    SignalKey::SignalKey() {}
    SignalKey::SignalKey(SDL_KeyboardEvent event)
        : _key(event.keysym.sym), _keyType(event.type) {}
    SignalKey::~SignalKey() {}

    //* -------------------- SIGNAL UNION -------------------- *//

    SignalUnion &SignalUnion::operator=(const SignalUnion &other)
    {
        std::memcpy(this, &other, sizeof(SignalUnion));
        return *this;
    }
    SignalUnion::SignalUnion(const SignalUnion &other)
    {
        std::memcpy(this, &other, sizeof(SignalUnion));
    }
    SignalUnion::SignalUnion() : customSignal(SignalCustom()) {}
    SignalUnion::SignalUnion(SignalKey keySignal) : keySignal(keySignal) {}
    SignalUnion::SignalUnion(SignalCustom customSignal) : customSignal(customSignal) {}
    SignalUnion::~SignalUnion() {}

    //* ----------------------- SIGNAL ----------------------- *//

    std::string Signal::getSignalTypeString()
    {
        switch (_signalType)
        {
        case SignalType::Custom:
            return std::string("Custom");
        default:
            return std::string();
        }
    }
    int Signal::getSignalId() { return _signalId; }
    void Signal::setSignalId(int signalId) { _signalId = signalId; }
    SignalType Signal::getSignalType() { return _signalType; }
    void Signal::setSignalType(SignalType signalType) { _signalType = signalType; }
    std::string Signal::getSignalName() { return _signalName; }
    void Signal::setSignalName(std::string signalName) { _signalName = signalName; }
    SignalUnion Signal::getSignal() { return _signal; }
    void Signal::setSignal(SignalUnion signal) { _signal = signal; }

    void Signal::updateSignal(SignalType signalType, int id, std::string name, ...)
    {
        // no error when lacking std::
        setSignalType(signalType);
        setSignalId(id);
        setSignalName(name);
        std::va_list args;
        va_start(args, signalType);
        switch (signalType)
        {
        case SignalType::Custom:
        {
            std::vector<int> targetList = va_arg(args, std::vector<int>);
            setSignal(SignalUnion(SignalCustom(
                targetList)));
            break;
        }
        default:
            break;
        }
        va_end(args);
    }
    Signal::Signal() : Signal(-1, SignalType::Custom, std::string(),
                              SignalUnion())
    {
    }
    Signal::Signal(const Signal &) = default;
    Signal::Signal(int signalId, SignalType signalType, std::string signalName,
                   SignalUnion signal)
        : _signalId(signalId), _signalType(signalType), _signalName(signalName),
          _signal(signal) {}
    Signal::~Signal() {}
    Signal Signal::parse(nlohmann::json root)
    {
        int id = root.at("signalId").get<int>();
        SignalType type = getSignalTypeFromString(
            root.at("SignalType").get<std::string>());
        std::string signalName = root.at("signalName").get<std::string>();
        SignalUnion signal_u;
        switch (type)
        {
        case SignalType::Custom:
            signal_u.customSignal = SignalCustom::parse(root);
            break;
        // key signal is system signal, will not be stored
        default:
            break;
        }
        return Signal(id, type, signalName, signal_u);
    }
    SignalType Signal::getSignalTypeFromString(std::string s)
    {
        if (s.compare("Custom"))
        {
            return SignalType::Custom;
        }
        else
            return SignalType::Custom;
    }
}