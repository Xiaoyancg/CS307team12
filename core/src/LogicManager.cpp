#pragma once
#include "LogicManager.h"
namespace Core
{

    //* ----------------------- SIGNAL ----------------------- *//

    Signal *LogicManager::createSignal()
    {
        _signalList.push_back(Signal());
        return &(_signalList.at(_signalList.size() - 1));
    }
    std::vector<Signal> *LogicManager::getSignals()
    {
        return &_signalList;
    }
    void LogicManager::sendSignal(Signal signal)
    {
        _currSignalList.push_back(signal);
    }
    void LogicManager::sendSignal(int signalId)
    {
        for (auto &signal : _signalList)
        {
            if (signal.getSignalId() == signalId)
            {
                sendSignal(signal);
            }
        }
    }

    //* ----------------------- SCRIPT ----------------------- *//

    Script *LogicManager::createScript()
    {
        _scriptList.push_back(Script());
        return &(_scriptList.at(_scriptList.size() - 1));
    }

    void LogicManager::sendScript(int scriptId)
    {
        for (auto &script : _scriptList)
        {
            if (script.getScriptId() == scriptId)
            {
                _currScriptList.push_back(&script);
            }
        }
    }

    //* -------------------- LOGICMANAGER -------------------- *//

    LogicManager LogicManager::parse(nlohmann::json root)
    {
        //TODO
        auto signalVector = root.at("signalList").get<std::vector<nlohmann::json>>();
        for (auto signal_js : signalVector)
        {
            _signalList.push_back(Signal::parse(signal_js));
        }
        auto logicVector = root.at("logicList").get<std::vector<nlohmann::json>>();
        for (auto logic_js : logicVector)
        {
        }
        auto scriptVector = root.at("scriptList").get<std::vector<nlohmann::json>>();
        for (auto script_js : scriptVector)
        {
            _scriptList.push_back(Script::parse(script_js));
        }
    }

    LogicManager::LogicManager() {}
    LogicManager::~LogicManager() {}
}