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
        for (auto &signal : _currSignalList)
        {
            switch (signal.getSignalType())
            {
            // key logic enable key logic when meets the key criteria
            case SignalType::Key:
                for (auto &logic : _currKeyLogicList)
                {
                    //TODO change get function to return reference
                    if (logic->getLogic().keyLogic.check(signal.getSignal().keySignal))
                    {
                        // TODO add bool in logic to enable remove function
                        // if(logic.getAction())
                        for (auto &scriptId : logic->getTargetScriptList())
                        {
                            sendScript(scriptId);
                        }
                    }
                }
                break;
            // custom signal enable all target scripts in target logics
            case SignalType::Custom:
                // custom signal check custom first then others
                for (auto &logic : _currCustomLogicList)
                {
                    for (auto &logicId : signal.getSignal().customSignal.getTargetLogicList())
                    {
                        if (logic->getLogicId() == logicId)
                        {
                            for (auto &scriptId : logic->getTargetScriptList())
                            {
                                sendScript(scriptId);
                            }
                        }
                    }
                }
                for (auto &logic : _currKeyLogicList)
                {
                    for (auto &logicId : signal.getSignal().customSignal.getTargetLogicList())
                    {
                        if (logic->getLogicId() == logicId)
                        {
                            for (auto &scriptId : logic->getTargetScriptList())
                            {
                                sendScript(scriptId);
                            }
                        }
                    }
                }
                break;

            default:
                break;
            }
        }
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

    Script *LogicManager::createScript(std::string name)
    {
        _scriptList.push_back(Script(name));
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
            switch (script->getScriptType())
            {
            case ScriptType::MoveConstantly:
                runMoveConstantly(script->getScript().scriptMoveConstantly);
                break;
            case ScriptType::Custom:
                runCustom(script->getScript().scriptCustom);
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

    void LogicManager::runMoveConstantly(ScriptMoveConstantly script)
    {
        if ((*_currPage)->getID() == script.getTargetPageId())
        {
            //TODO: add entity id in parse and in Entity constructor
            for (auto &entity : (*_currPage)->getEntityList())
            {
                for (auto &entityId : script.getTargetEntityList())
                {
                    if (entity->getEntityId() == entityId)
                    {
                        entity->setLocation(entity->getLocation() + script.getMovement());
                    }
                }
            }
        }
    }

    void LogicManager::runCustom(ScriptCustom script)
    {
        // true to send
        for (auto signalId : script.getAddTargetSignalList())
        {
            sendSignal(signalId);
        }
        for (auto logicId : script.getAddTargetLogicList())
        {
            sendLogic(logicId);
        }
        for (auto scriptId : script.getAddTargetScriptList())
        {
            sendScript(scriptId);
        }
        // false to remove
        for (auto signalId : script.getRemoveTargetSignalList())
        {
            removeSignal(signalId);
        }
        for (auto logicId : script.getRemoveTargetLogicList())
        {
            removeLogic(logicId);
        }
        for (auto scriptId : script.getRemoveTargetScriptList())
        {
            removeScript(scriptId);
        }
    }
    //* -------------------- LOGICMANAGER -------------------- *//
    LogicManager LogicManager::parse(nlohmann::json root)
    {
        //TODO
        // FIXME auto signalVector = root.at("signalList")
        //.get<std::vector<nlohmann::json>>();
        //for (auto signal_js : signalVector)
        //{
        //    //FIXME _signalList.push_back(Signal::parse(signal_js));
        //}
        ////FIXME auto logicVector = root.at("logicList")
        ////.get<std::vector<nlohmann::json>>();
        //for (auto logic_js : logicVector)
        //{
        //    // FIXME
        //}
        auto scriptVector = root.at("scriptList")
                                .get<std::vector<nlohmann::json>>();
        for (auto script_js : scriptVector)
        {
            _scriptList.push_back(Script::parse(script_js));
        }
        return *this;
    }

    LogicManager::LogicManager() {}
    LogicManager::LogicManager(std::vector<Page *> *pageList)
        : _pageList(pageList) {}

    LogicManager::~LogicManager() {}
}