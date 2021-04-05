#include "Script.h"
namespace Core
{

    //* ----------------------- CUSTOM ----------------------- *//

    std::vector<int> ScriptCustom::getTargetSignalList()
    {
        return _targetSignalList;
    }
    std::vector<int> ScriptCustom::getTargetLogicList()
    {
        return _targetLogicList;
    }
    std::vector<int> ScriptCustom::getTargetScriptList()
    {
        return _targetScriptList;
    }

    void ScriptCustom::setTargetSignalList(std::vector<int> targetSignalList)
    {
        _targetSignalList = targetSignalList;
    }
    void ScriptCustom::setTargetLogicList(std::vector<int> targetLogicList)
    {
        _targetLogicList = targetLogicList;
    }
    void ScriptCustom::setTargetScriptList(std::vector<int> targetScriptList)
    {
        _targetScriptList = targetScriptList;
    }
    ScriptCustom::ScriptCustom() : ScriptCustom(std::vector<int>(),
                                                std::vector<int>(),
                                                std::vector<int>()) {}
    ScriptCustom::ScriptCustom(std::vector<int> targetSignalList,
                               std::vector<int> targetLogicList,
                               std::vector<int> targetScriptList)
        : _targetLogicList(targetLogicList),
          _targetScriptList(targetScriptList),
          _targetSignalList(targetSignalList) {}

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

    ScriptUnion::ScriptUnion() : ScriptUnion(ScriptCustom()) {}
    ScriptUnion::ScriptUnion(ScriptCustom scriptCustom)
        : scriptCustom(scriptCustom) {}
    ScriptUnion::ScriptUnion(ScriptMoveConstantly scriptMoveConstantly)
        : scriptConstantly(scriptMoveConstantly) {}
    ScriptUnion::~ScriptUnion() {}

    //* ----------------------- SCRIPT ----------------------- *//

}