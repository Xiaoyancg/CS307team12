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
        std::vector<int> getTargetSignalList();
        std::vector<int> getTargetLogicList();
        std::vector<int> getTargetScriptList();
        void setTargetSignalList(std::vector<int> targetSignalList);
        void setTargetLogicList(std::vector<int> targetLogicList);
        void setTargetScriptList(std::vector<int> targetScriptList);

        ScriptCustom();

        ScriptCustom(std::vector<int> targetSignalList,
                     std::vector<int> targetLogicList,
                     std::vector<int> targetScriptList);
        ~ScriptCustom() = default;
    };

    class ScriptMoveConstantly
    {
    private:
        glm::vec2 _movement;
        std::vector<int> _targetEntityList;
        int _targetPage;

    public:
        int getTargetPage();
        void setTargetPage(int targetPage);
        std::vector<int> getTargetEntityList();
        void setTargetEntityList(std::vector<int> targetEntityList);
        glm::vec2 getMovement();
        void setMovement(glm::vec2 movement);

        ScriptMoveConstantly();
        ScriptMoveConstantly(int targetPage,
                             std::vector<int> targetEntityList,
                             glm::vec2 movement);
        ~ScriptMoveConstantly() = default;
    };

    //* -------------------- ANCHOR SCRIPT ------------------- *//

    union ScriptUnion
    {
        ScriptCustom scriptCustom;
        ScriptMoveConstantly scriptConstantly;
        ScriptUnion();
        ScriptUnion(ScriptCustom scriptCustom);
        ScriptUnion(ScriptMoveConstantly scriptMoveConstantly);
        ~ScriptUnion();
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