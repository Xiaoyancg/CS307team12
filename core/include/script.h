#pragma once
#include <vector>
#include <glm/glm.hpp>
namespace Core
{
    enum class ScriptType
    {
        Custom = 0,
        MoveConstantly = 1,

    };

    //* ----------------- ANCHOR SCRIPT TYPES ---------------- *//

    class ScriptCustom
    {
    private:
        std::vector<int> _targetSignalList;
        std::vector<int> _targetLogicList;
        std::vector<int> _targetScriptList;

    public:
        std::vector<int> getTargetSignalList() { return _targetSignalList; }
        std::vector<int> getTargetLogicList() { return _targetLogicList; }
        std::vector<int> getTargetScriptList() { return _targetScriptList; }
        void setTargetSignalList(std::vector<int> targetSignalList)
        {
            _targetSignalList = targetSignalList;
        }
        void setTargetLogicList(std::vector<int> targetLogicList)
        {
            _targetLogicList = targetLogicList;
        }
        void setTargetScriptList(std::vector<int> targetScriptList)
        {
            _targetScriptList = targetScriptList;
        }

        ScriptCustom() : ScriptCustom(std::vector<int>(),
                                      std::vector<int>(),
                                      std::vector<int>()) {}

        ScriptCustom(std::vector<int> targetSignalList,
                     std::vector<int> targetLogicList,
                     std::vector<int> targetScriptList)
            : _targetLogicList(targetLogicList),
              _targetScriptList(targetScriptList),
              _targetSignalList(targetSignalList) {}
        ~ScriptCustom() = default;
    };

    class ScriptMoveConstantly
    {
    private:
        glm::vec2 _movement;
        std::vector<int> _targetEntityList;
        int _targetPage;

    public:
        int getTargetPage() { return _targetPage; }
        void setTargetPage(int targetPage) { _targetPage = targetPage; }
        std::vector<int> getTargetEntityList() { return _targetEntityList; }
        void setTargetEntityList(std::vector<int> targetEntityList)
        {
            _targetEntityList = targetEntityList;
        }
        glm::vec2 getMovement() { return _movement; }
        void setMovement(glm::vec2 movement) { _movement = movement; }

        ScriptMoveEntitiesConstantly()
            : ScriptMoveEntitiesConstantly(-1,
                                           std::vector<int>(),
                                           glm::vec2(0, 0)) {}
        ScriptMoveEntitiesConstantly(int targetPage,
                                     std::vector<int> targetEntityList,
                                     glm::vec2 movement)
            : _movement(movement),
              _targetEntityList(targetEntityList),
              _targetPage(targetPage) {}
        ~ScriptMoveConstantly() = default;
    };

    //* -------------------- ANCHOR SCRIPT ------------------- *//

    union ScriptUnion
    {
        ScriptCustom scriptCustom;
        ScriptMoveConstantly scriptConstantly;
        ScriptUnion() : ScriptUnion(ScriptCustom()) {}
        ScriptUnion(const Script &) = default;
        ScriptUnion(ScriptCustom scriptCustom) : scriptCustom(scriptCustom) {}
        ScriptUnion(ScriptMoveConstantly scriptMoveConstantly)
            : scriptConstantly(scriptMoveConstantly) {}
        ~Script() = default;
    };

    class script
    {
    private:
        int _scriptId;
        ScriptType _scriptType;
        ScriptUnion _script;

    public:
        int getScriptId() { return _scriptId; }
        void setScriptId(int scriptId) { _scriptId = scriptId; }
        ScriptType getScriptType() { return _scriptType; }
        void setScriptType(ScriptType scriptType) { _scriptType = scriptType; }
        ScriptUnion getScript(){return Script}

        script() : script(-1, ScriptType::Custom, ScriptUnion())
        {
        }

        script(int scriptId, ScriptType scriptType, ScriptUnion script)
            : _scriptId(scriptId), _scriptType(scriptType), _script(script) {}

        ~script() = default
    };

}