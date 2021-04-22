#include <cstdarg>
#include "Script.h"
#include <cstring>

namespace Core
{

    static std::unordered_map<ScriptType, std::string> scriptTypeStringList = {
        {ScriptType::Custom, "Custom"},
        {ScriptType::MoveConstantly, "MoveConstantly"},
        {ScriptType::Bounce, "Bounce"},
        {ScriptType::SwitchPage, "SwitchPage"},
        {ScriptType::GameEnd, "GameEnd"}};
    std::string getScriptTypeStringByScriptType(ScriptType type)
    {
        // no error should be found here
        return scriptTypeStringList.at(type);
    }
    //* ----------------------- CUSTOM ----------------------- *//

    std::vector<int> ScriptCustom::getAddTargetSignalList()
    {
        return _addTargetSignalList;
    }
    std::vector<int> ScriptCustom::getAddTargetLogicList()
    {
        return _addTargetLogicList;
    }
    std::vector<int> ScriptCustom::getAddTargetScriptList()
    {
        return _addTargetScriptList;
    }
    void ScriptCustom::setAddTargetSignalList(std::vector<int> targetSignalList)
    {
        _addTargetSignalList = targetSignalList;
    }
    void ScriptCustom::setAddTargetLogicList(std::vector<int> targetLogicList)
    {
        _addTargetLogicList = targetLogicList;
    }
    void ScriptCustom::setAddTargetScriptList(std::vector<int> targetScriptList)
    {
        _addTargetScriptList = targetScriptList;
    }
    std::vector<int> ScriptCustom::getRemoveTargetSignalList()
    {
        return _removeTargetSignalList;
    }
    std::vector<int> ScriptCustom::getRemoveTargetLogicList()
    {
        return _removeTargetLogicList;
    }
    std::vector<int> ScriptCustom::getRemoveTargetScriptList()
    {
        return _removeTargetScriptList;
    }
    void ScriptCustom::setRemoveTargetSignalList(std::vector<int> targetSignalList)
    {
        _removeTargetSignalList = targetSignalList;
    }
    void ScriptCustom::setRemoveTargetLogicList(std::vector<int> targetLogicList)
    {
        _removeTargetLogicList = targetLogicList;
    }
    void ScriptCustom::setRemoveTargetScriptList(std::vector<int> targetScriptList)
    {
        _removeTargetScriptList = targetScriptList;
    }

    ScriptCustom::ScriptCustom() : ScriptCustom(std::vector<int>(),
                                                std::vector<int>(),
                                                std::vector<int>(),
                                                std::vector<int>(),
                                                std::vector<int>(),
                                                std::vector<int>()) {}

    ScriptCustom::ScriptCustom(std::vector<int> addTargetSignalList,
                               std::vector<int> addTargetLogicList,
                               std::vector<int> addTargetScriptList,
                               std::vector<int> removeTargetSignalList,
                               std::vector<int> removeTargetLogicList,
                               std::vector<int> removeTargetScriptList)
        : _addTargetSignalList(addTargetSignalList),
          _addTargetLogicList(addTargetLogicList),
          _addTargetScriptList(addTargetScriptList),
          _removeTargetSignalList(removeTargetSignalList),
          _removeTargetLogicList(removeTargetLogicList),
          _removeTargetScriptList(removeTargetScriptList) {}
    //* ------------------- MOVE CONSTANTLY ------------------ *//

    int ScriptMoveConstantly::getTargetPageId() { return _targetPage; }
    void ScriptMoveConstantly::setTargetPage(int targetPage)
    {
        _targetPage = targetPage;
    }
    std::vector<int> ScriptMoveConstantly::getTargetEntityList()
    {
        return _targetEntityList;
    }
    void ScriptMoveConstantly::setTargetEntityList(
        std::vector<int> targetEntityList)
    {
        _targetEntityList = targetEntityList;
    }
    glm::vec2 ScriptMoveConstantly::getMovement() { return _movement; }
    void ScriptMoveConstantly::setMovement(glm::vec2 movement)
    {
        _movement = movement;
    }

    ScriptMoveConstantly::ScriptMoveConstantly()
        : ScriptMoveConstantly(-1,
                               std::vector<int>(),
                               glm::vec2(0, 0)) {}
    ScriptMoveConstantly::ScriptMoveConstantly(
        int targetPage,
        std::vector<int> targetEntityList,
        glm::vec2 movement)
        : _movement(movement),
          _targetEntityList(targetEntityList),
          _targetPage(targetPage) {}

    //* -------------------- SCRIPT UNION -------------------- *//

    ScriptUnion::ScriptUnion(const ScriptUnion &other)
    {
        std::memcpy(this, &other, sizeof(ScriptUnion));
    }
    ScriptUnion &ScriptUnion::operator=(const ScriptUnion &other)
    {
        std::memcpy(this, &other, sizeof(ScriptUnion));
        return *this;
    }
    ScriptUnion::ScriptUnion() : ScriptUnion(ScriptCustom()) {}
    ScriptUnion::ScriptUnion(ScriptCustom scriptCustom)
        : scriptCustom(scriptCustom) {}
    ScriptUnion::ScriptUnion(ScriptMoveConstantly scriptMoveConstantly)
        : scriptMoveConstantly(scriptMoveConstantly) {}
    ScriptUnion::~ScriptUnion() {}

    //* ----------------------- SCRIPT ----------------------- *//

    std::string Script::getScriptTypeString()
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
    int Script::getScriptId() { return _scriptId; }
    void Script::setScriptId(int scriptId) { _scriptId = scriptId; }
    ScriptType Script::getScriptType() { return _scriptType; }
    void Script::setScriptType(ScriptType scriptType)
    {
        _scriptType = scriptType;
    }
    std::string Script::getScriptName() { return _scriptName; }
    void Script::setScriptName(std::string scriptname)
    {
        _scriptName = scriptname;
    }
    ScriptUnion Script::getScript() { return _script; }
    void Script::setScript(ScriptUnion script) { _script = script; }

    Script Script::parse(nlohmann::json root)
    {
        // TODO
        Script s;
        s.setScriptName(root.at("ScriptName").get<std::string>());
        s.setScriptId(root.at("scriptId").get<int>());
        s.setScriptType(
            getScriptTypeFromString(root.at("ScriptType").get<std::string>()));
        nlohmann::json script = root.at("script").get<nlohmann::json>();
        switch (s.getScriptType())
        {
        case ScriptType::Custom:
            s.setScript(ScriptUnion(ScriptCustom(
                script.at("addTargetSignalList").get<std::vector<int>>(),
                script.at("addTargetLogicList").get<std::vector<int>>(),
                script.at("addTargetScriptList").get<std::vector<int>>(),
                script.at("removeTargetSignalList").get<std::vector<int>>(),
                script.at("removeTargetLogicList").get<std::vector<int>>(),
                script.at("removeTargetScriptList").get<std::vector<int>>())));
            break;
        case ScriptType::MoveConstantly:
        {
            std::vector tv = (script.at("movement").get<std::vector<int>>());
            glm::vec2 movement = glm::vec2(tv.at(0), tv.at(1));
            s.setScript(ScriptUnion(ScriptMoveConstantly(
                script.at("targetPageId"),
                script.at("targetEntityList"),
                movement)));
            break;
        }
        case ScriptType::Bounce:
        {
            std::vector tv = (script.at("movement").get<std::vector<int>>());
            glm::vec2 movement = glm::vec2(tv.at(0), tv.at(1));
            s.setScript(ScriptUnion(ScriptMoveConstantly(
                script.at("targetPageId"),
                script.at("targetEntityList"),
                movement)));
            break;
        }
        default:
            break;
        }
        return s;
    }

    ScriptType Script::getScriptTypeFromString(std::string typeString)
    {
        if (typeString.compare("Custom") == 0)
        {
            return ScriptType::Custom;
        }
        else if (typeString.compare("MoveConstantly") == 0)
        {
            return ScriptType::MoveConstantly;
        }
    }
    void Script::updateScript(ScriptType scriptType, int id, std::string name...)
    {
        std::va_list args;
        setScriptType(scriptType);
        setScriptId(id);
        setScriptName(name);
        va_start(args, name);
        switch (scriptType)
        {
        case ScriptType::Custom:
        {
            std::vector<int> addsig = va_arg(args, std::vector<int>);
            std::vector<int> addlog = va_arg(args, std::vector<int>);
            std::vector<int> addscr = va_arg(args, std::vector<int>);
            std::vector<int> remsig = va_arg(args, std::vector<int>);
            std::vector<int> remlog = va_arg(args, std::vector<int>);
            std::vector<int> remscr = va_arg(args, std::vector<int>);
            setScript(
                ScriptUnion(
                    ScriptCustom(addsig, addlog, addscr,
                                 remsig, remlog, remscr)));
            break;
        }
        case ScriptType::MoveConstantly:
        {
            int pageId = va_arg(args, int);
            std::vector<int> entityList = va_arg(args, std::vector<int>);
            glm::vec2 movement = va_arg(args, glm::vec2);
            setScript(
                ScriptUnion(
                    ScriptMoveConstantly(
                        pageId,
                        entityList,
                        movement)));

            break;
        }
        default:
            break;
        }
        va_end(args);
    }

    Script::Script() : Script(-1, ScriptType::Custom, std::string(),
                              ScriptUnion())
    {
    }
    Script::Script(std::string name) : Script(-1, ScriptType::Custom, name, ScriptUnion()) {}

    Script::Script(int scriptId, ScriptType scriptType, std::string scriptName,
                   ScriptUnion script)
        : _scriptId(scriptId), _scriptType(scriptType), _scriptName(scriptName),
          _script(script) {}

    Script::~Script() {}
}