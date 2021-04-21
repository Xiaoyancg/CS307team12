#include <cstdarg>
#include <cstring>
#include "Logic.h"

namespace Core
{

    //* ----------------------- CUSTOM ----------------------- *//

    LogicCustom::LogicCustom() {}
    LogicCustom::~LogicCustom() {}

    //* ------------------------- KEY ------------------------ *//

    bool LogicKey::check(SignalKey keySignal)
    {
        return keySignal.getKey() == _key && keySignal.getKeyType() == _keyType;
    }
    LogicKey::LogicKey() : LogicKey(SDLK_SPACE, SDL_PRESSED) {}
    LogicKey::LogicKey(SDL_Keycode key, Uint32 keyType)
        : _key(key), _keyType(keyType) {}
    LogicKey::~LogicKey() {}

    //* --------------------- LOGIC UNION -------------------- *//
    LogicUnion &LogicUnion::operator=(const LogicUnion &other)
    {
        std::memcpy(this, &other, sizeof(LogicUnion));
        return *this;
    }
    LogicUnion::LogicUnion(const LogicUnion &other)
    {
        std::memcpy(this, &other, sizeof(LogicUnion));
    }
    LogicUnion::LogicUnion() : LogicUnion(LogicCustom()) {}
    LogicUnion::LogicUnion(LogicCustom customLogic)
        : customLogic(customLogic) {}
    LogicUnion::LogicUnion(LogicKey keyLogic)
        : keyLogic(keyLogic) {}
    LogicUnion::~LogicUnion() {}

    //* ------------------------ LOGIC ----------------------- *//

    void Logic::setLogicId(int logicId)
    {
        _logicId = logicId;
    }
    int Logic::getLogicId()
    {
        return _logicId;
    }
    void Logic::setSignalType(SignalType signalType)
    {
        _signalType = signalType;
    }
    SignalType Logic::getSignalType()
    {
        return _signalType;
    }
    void Logic::setScriptType(ScriptType scriptType)
    {
        _scriptType = scriptType;
    }
    ScriptType Logic::getScriptType()
    {
        return _scriptType;
    }
    void Logic::setLogicName(std::string logicName)
    {
        _logicName = logicName;
    }
    std::string Logic::getLogicName()
    {
        return _logicName;
    }
    void Logic::setTargetScriptList(std::vector<int> targetScriptList)
    {
        _targetScriptList = targetScriptList;
    }
    std::vector<int> Logic::getTargetScriptList()
    {
        return _targetScriptList;
    }
    void Logic::setLogic(LogicUnion logic)
    {
        _logic = logic;
    }
    LogicUnion Logic::getLogic()
    {
        return _logic;
    }

    void Logic::updateLogic(SignalType signalType, ...)
    {
        std::va_list args;
        va_start(args, signalType);
        setSignalType(signalType);
        setLogicId(va_arg(args, int));
        setScriptType(va_arg(args, ScriptType));
        setLogicName(va_arg(args, std::string));
        setTargetScriptList(va_arg(args, std::vector<int>));
        switch (signalType)
        {
        case SignalType::Custom:
            setLogic(
                LogicUnion(
                    LogicCustom()));
            break;
        case SignalType::Key:
            setLogic(
                LogicUnion(
                    LogicKey(
                        va_arg(args, SDL_Keycode),
                        va_arg(args, Uint32))));
            break;

        default:
            break;
        }
        va_end(args);
    }
    Logic Logic::parse(nlohmann::json root)
    {
        //TODO
        return Logic();
    }

    Logic::Logic() : Logic(-1, SignalType::Custom, ScriptType::Custom,
                           std::string(), std::vector<int>(), LogicUnion()) {}
    Logic::Logic(int logicId, SignalType signalType, ScriptType scriptType,
                 std::string logicName, std::vector<int> targetScriptList,
                 LogicUnion logic)
        : _logicId(logicId), _signalType(signalType), _scriptType(scriptType),
          _logicName(logicName), _targetScriptList(targetScriptList),
          _logic(logic) {}
    Logic::~Logic() {}
}