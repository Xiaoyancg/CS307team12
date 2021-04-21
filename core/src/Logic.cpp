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
    SDL_Keycode LogicKey::getKey()
    {
        return _key;
    }
    void LogicKey::setKey(SDL_Keycode key)
    {
        _key = key;
    }
    Uint32 LogicKey::getKeyType()
    {
        return _keyType;
    }
    void LogicKey::setKeyType(Uint32 keyType)
    {
        _keyType = keyType;
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
    std::string Logic::getSignalTypeString()
    {
        switch (_signalType)
        {
        case SignalType::Custom:
            return std::string("Custom");
        default:
            return std::string();
        }
    }
    std::string Logic::getScriptTypeString()
    {
        switch (_scriptType)
        {
        case ScriptType::Custom:
            return std::string("Custom");
            break;
        case ScriptType::MoveConstantly:
            return std::string("MoveConstantly");
            break;

        default:
            return std::string();
            break;
        }
    }

    void Logic::updateLogic(SignalType signalType, ScriptType scriptType, int id, std::string name, std::vector<int> targetList, ...)
    {
        setSignalType(signalType);
        setScriptType(scriptType);
        setLogicId(id);
        setLogicName(name);
        setTargetScriptList(targetList);
        std::va_list args;
        va_start(args, targetList);
        switch (signalType)
        {
        case SignalType::Custom:
            setLogic(
                LogicUnion(
                    LogicCustom()));
            break;
        case SignalType::Key:
        {
            SDL_Keycode keycode = va_arg(args, SDL_Keycode);
            Uint32 keyType = va_arg(args, Uint32);
            setLogic(
                LogicUnion(
                    LogicKey(keycode, keyType)));
            break;
        }

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