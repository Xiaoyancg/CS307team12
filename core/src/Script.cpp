#include <cstdarg>
#include "Script.h"
#include <cstring>

namespace Core
{

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

    int ScriptMoveConstantly::getTargetPage() { return _targetPage; }
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

        default:
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
    }
    void Script::updateScript(ScriptType scriptType, ...)
    {
        std::va_list args;
        va_start(args, scriptType);
        setScriptId(va_arg(args, int));
        setScriptType(scriptType);
        setScriptName(va_arg(args, std::string));
        switch (scriptType)
        {
        case ScriptType::Custom:
            setScript(
                ScriptUnion(
                    ScriptCustom(
                        va_arg(args, std::vector<int>),
                        va_arg(args, std::vector<int>),
                        va_arg(args, std::vector<int>),
                        va_arg(args, std::vector<int>),
                        va_arg(args, std::vector<int>),
                        va_arg(args, std::vector<int>))));
            break;
        case ScriptType::MoveConstantly:
            setScript(
                ScriptUnion(
                    ScriptMoveConstantly(
                        va_arg(args, int),
                        va_arg(args, std::vector<int>),
                        va_arg(args, glm::vec2))));
            break;
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