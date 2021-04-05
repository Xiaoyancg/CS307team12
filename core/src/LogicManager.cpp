#pragma once
#include "LogicManager.h"
namespace Core
{

    //* ----------------------- SIGNAL ----------------------- *//

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
    void LogicManager::removeSignal(int signalId)
    {
        for (std::vector<Signal>::iterator it = _currSignalList.begin();
             it != _currSignalList.end(); it++)
        {
            if (it->getSignalId() == signalId)
            {
                _currSignalList.erase(it);
                return;
            }
        }
    }

    Signal *LogicManager::createSignal()
    {
        _signalList.push_back(Signal());
        return &(_signalList.at(_signalList.size() - 1));
    }

    void LogicManager::deleteSignal(int signalId)
    {
        for (std::vector<Signal>::iterator it = _signalList.begin();
             it != _signalList.end(); ++it)
        {
            if (it->getSignalId() == signalId)
            {
                _signalList.erase(it);
                return;
            }
        }
    }
    std::vector<Signal> *LogicManager::getSignalList()
    {
        return &_signalList;
    }
    void LogicManager::checkCurrSignalList()
    {
        //TODO
    }

    //* ------------------------ LOGIC ----------------------- *//
    void LogicManager::sendLogic(int logicId)
    {
        for (auto &logic : _logicList)
        {
            if (logic.getLogicId() == logicId)
            {
                switch (logic.getSignalType())
                {
                case SignalType::Key:
                    _currKeyLogicList.push_back(&logic);
                    break;
                case SignalType::Custom:
                    _currCustomLogicList.push_back(&logic);
                    break;

                default:
                    break;
                }
                return;
            }
        }
    }
    void LogicManager::removeLogic(int logicId)
    {
        for (std::vector<Logic *>::iterator it = _currKeyLogicList.begin();
             it != _currKeyLogicList.end(); ++it)
        {
            if ((*it)->getLogicId() == logicId)
            {
                _currKeyLogicList.erase(it);
                return;
            }
        }
        for (std::vector<Logic *>::iterator it = _currCustomLogicList.begin();
             it != _currCustomLogicList.end(); ++it)
        {
            if ((*it)->getLogicId() == logicId)
            {
                _currCustomLogicList.erase(it);
                return;
            }
        }
    }

    Logic *LogicManager::createLogic()
    {
        _logicList.push_back(Logic());
        return &_logicList.at(_logicList.size() - 1);
    }

    void LogicManager::deleteLogic(int logicId)
    {
        for (std::vector<Logic>::iterator it = _logicList.begin();
             it != _logicList.end(); ++it)
        {
            if (it->getLogicId() == logicId)
            {
                _logicList.erase(it);
                return;
            }
        }
    }
    std::vector<Logic> *LogicManager::getLogicList()
    {
        return &_logicList;
    }

    //* ----------------------- SCRIPT ----------------------- *//

    void LogicManager::sendScript(int scriptId)
    {
        // TODO: no duplicate check
        for (auto &script : _scriptList)
        {
            if (script.getScriptId() == scriptId)
            {
                // use different level to avoid error
                int level = static_cast<int>(script.getScriptType());
                if (level > GAMELEVEL)
                {
                    _currGameScriptList.push_back(&script);
                }
                else if (level > PAGELEVEL)
                {
                    _currPageScriptList.push_back(&script);
                }
                else
                {
                    _currEntityScriptList.push_back(&script);
                }
            }
        }
    }

    void LogicManager::removeScript(int scriptId)
    {
        for (std::vector<Script *>::iterator it =
                 _currEntityScriptList.begin();
             it != _currEntityScriptList.end(); ++it)
        {
            if ((*it)->getScriptId() == scriptId)
            {
                _currEntityScriptList.erase(it);
                return;
            }
        }
        for (std::vector<Script *>::iterator it =
                 _currPageScriptList.begin();
             it != _currPageScriptList.end(); ++it)
        {
            if ((*it)->getScriptId() == scriptId)
            {
                _currPageScriptList.erase(it);
                return;
            }
        }
        for (std::vector<Script *>::iterator it =
                 _currGameScriptList.begin();
             it != _currGameScriptList.end(); ++it)
        {
            if ((*it)->getScriptId() == scriptId)
            {
                _currGameScriptList.erase(it);
                return;
            }
        }
    }

    Script *LogicManager::createScript()
    {
        _scriptList.push_back(Script());
        return &(_scriptList.at(_scriptList.size() - 1));
    }

    void LogicManager::deleteScript(int scriptId)
    {
        for (std::vector<Script>::iterator it = _scriptList.begin();
             it != _scriptList.end(); ++it)
        {
            if (it->getScriptId() == scriptId)
            {
                _scriptList.erase(it);
                return;
            }
        }
    }

    std::vector<Script> *LogicManager::getScriptList()
    {
        return &_scriptList;
    }

    void LogicManager::runCurrScriptList()
    {
        // FIXME for now the script are permanent until a script remove it
        //TODO
        for (auto &script : _currEntityScriptList)
        {
            ScriptMoveConstantly mcs;
            ScriptCustom cs;
            switch (script->getScriptType())
            {
            case ScriptType::MoveConstantly:
                mcs = script->getScript().scriptMoveConstantly;
                if ((*_currPage)->getID() == mcs.getTargetPage())
                {
                    //TODO: add entity id in parse and in Entity constructor
                    for (auto &entity : (*_currPage)->getEntityList())
                    {
                        for (auto &entityId : mcs.getTargetEntityList())
                        {
                            if (entity->getEntityId() == entityId)
                            {
                                entity->setLocation(entity->getLocation() + mcs.getMovement());
                            }
                        }
                    }
                }
                break;
            case ScriptType::Custom:
                cs = script->getScript().scriptCustom;
                if (cs.getAction())
                {
                    // true to send
                    for (auto signalId : cs.getTargetSignalList())
                    {
                        sendSignal(signalId);
                    }
                    for (auto logicId : cs.getTargetLogicList())
                    {
                        sendLogic(logicId);
                    }
                    for (auto scriptId : cs.getTargetScriptList())
                    {
                        sendScript(scriptId);
                    }
                }
                else
                {
                    // false to remove
                    for (auto signalId : cs.getTargetSignalList())
                    {
                        removeSignal(signalId);
                    }
                    for (auto logicId : cs.getTargetLogicList())
                    {
                        removeLogic(logicId);
                    }
                    for (auto scriptId : cs.getTargetScriptList())
                    {
                        removeScript(scriptId);
                    }
                }
                break;

            default:
                break;
            }
        }

        for (auto &script : _currPageScriptList)
        {
        }

        for (auto &script : _currGameScriptList)
        {
        }
    }

    //* -------------------- LOGICMANAGER -------------------- *//
    LogicManager LogicManager::parse(nlohmann::json root)
    {
        //TODO
        auto signalVector = root.at("signalList")
                                .get<std::vector<nlohmann::json>>();
        for (auto signal_js : signalVector)
        {
            _signalList.push_back(Signal::parse(signal_js));
        }
        auto logicVector = root.at("logicList")
                               .get<std::vector<nlohmann::json>>();
        for (auto logic_js : logicVector)
        {
        }
        auto scriptVector = root.at("scriptList")
                                .get<std::vector<nlohmann::json>>();
        for (auto script_js : scriptVector)
        {
            _scriptList.push_back(Script::parse(script_js));
        }
        return *this;
    }

    LogicManager::LogicManager() {}
    LogicManager::LogicManager(std::vector<Page *> *pageList, Page **currPage)
        : _pageList(pageList), _currPage(currPage) {}

    LogicManager::~LogicManager() {}
}