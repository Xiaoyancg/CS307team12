#include "LogicManager.h"
namespace Core
{
    int LogicManager::sendSignal(SignalType type, Signal &signal_ref)
    {
        switch (type)
        {
        case SignalType::Key:
            mcurrSignals[std::get<KeySignal>(signal_ref).getSignalName()] = signal_ref;
            break;

        default:
            break;
        }
        return 0;
    }
    int LogicManager::sendScript(ScriptType type, Script *script_ptr)
    {
        switch (type)
        {
        case ScriptType::MoveEntitiesConstantly:
            ScriptMoveEntityConstantly &mec =
                std::get<ScriptMoveEntityConstantly>(*script_ptr);
            mcurrEntityScripts[mec.getScriptName()] = script_ptr;
            break;

        default:
            break;
        }
        return 0;
    }
    void LogicManager::check(Signal &signal_ref)
    {
        std::visit([&](auto &&signal) {
            switch (signal.getType())
            {
            case SignalType::Key:
                for (auto logicpair : mcurrKeyLogics)
                {
                    KeyLogic &keyLogic = std::get<KeyLogic>(*logicpair.second);
                    if (keyLogic.check(signal_ref))
                    {
                        sendScript(
                            keyLogic.getScriptType(),
                            keyLogic.getScriptPtr());
                    }
                }
                break;

            default:
                break;
            }
        },
                   signal_ref);
    }
    void LogicManager::signalLoop()
    {
        for (auto signalpair : mcurrSignals)
        {
            check(signalpair.second);
        }
    }
    void LogicManager::scriptLoop()
    {
        // only have entity script list rn
        for (auto scriptpair : mcurrEntityScripts)
        {
            std::visit([&](auto &&script) { script.run(); },
                       *(scriptpair.second));
        }
    }
    //* ------------------- ANCHOR CREATION ------------------ *//
    int LogicManager::createMoveEntitiesConstantlyScript(
        std::string &scriptName_ref,
        std::vector<std::string> targetList,
        std::string &targetPage_ref,
        glm::vec2 &movement_ref)
    {
        // used name
        if (mscripts.find(scriptName_ref) != mscripts.end())
        {
            return 1;
        }

        mscripts.insert(
            std::pair<std::string, Script>(
                scriptName_ref,
                ScriptMoveEntityConstantly(mpageManager_ptr,
                                           scriptName_ref,
                                           targetList,
                                           targetPage_ref,
                                           movement_ref)));
        return 0;
    }

    int LogicManager::createKeyLogic(std::string &logicName_ref,
                                     SignalType signalType,
                                     ScriptType scriptType,
                                     std::string &scriptName_ref,
                                     SDL_Keycode key,
                                     Uint32 keyType)
    {
        // used name
        if (mlogics.find(logicName_ref) != mlogics.end())
        {
            return 1;
        }
        Script *script_ptr;
        try
        {
            script_ptr = &mscripts.at(scriptName_ref);
        }
        catch (const std::exception &e)
        {
            return 2;
        }

        mlogics.insert(
            std::pair<std::string, Logic>(
                logicName_ref,
                KeyLogic(logicName_ref, signalType, scriptType,
                         script_ptr, key, keyType)));
        return 0;
    }

}