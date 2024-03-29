#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#define PAGELEVEL 9999
#define GAMELEVEL 19999
namespace Core
{
    enum class ScriptType
    {
        // run from 0 -> 20000 to avoid error
        // logic level (base level)
        Custom = 0,
        // entity level
        MoveConstantly = 1,
        // page level
        SwitchPage = 10000,
        // game level
        GameEnd = 20000,

    };
    extern std::unordered_map<ScriptType, std::string> scriptTypeStringList;
    std::string getScriptTypeStringByScriptType(ScriptType type);

    //* ----------------- ANCHOR SCRIPT TYPES ---------------- *//

    //* ----------------------- CUSTOM ---------------------- *//

    class ScriptCustom
    {
    private:
        std::vector<int> _addTargetSignalList;
        std::vector<int> _addTargetLogicList;
        std::vector<int> _addTargetScriptList;
        std::vector<int> _removeTargetSignalList;
        std::vector<int> _removeTargetLogicList;
        std::vector<int> _removeTargetScriptList;

    public:
        std::vector<int> getAddTargetSignalList();
        std::vector<int> getAddTargetLogicList();
        std::vector<int> getAddTargetScriptList();
        void setAddTargetSignalList(std::vector<int> addTargetSignalList);
        void setAddTargetLogicList(std::vector<int> addTargetLogicList);
        void setAddTargetScriptList(std::vector<int> addTargetScriptList);
        std::vector<int> getRemoveTargetSignalList();
        std::vector<int> getRemoveTargetLogicList();
        std::vector<int> getRemoveTargetScriptList();
        void setRemoveTargetSignalList(std::vector<int> removeTargetSignalList);
        void setRemoveTargetLogicList(std::vector<int> removeTargetLogicList);
        void setRemoveTargetScriptList(std::vector<int> removeTargetScriptList);

        ScriptCustom();
        ScriptCustom(std::vector<int> addTargetSignalList,
                     std::vector<int> addTargetLogicList,
                     std::vector<int> addTargetScriptList,
                     std::vector<int> removeTargetSignalList,
                     std::vector<int> removeTargetLogicList,
                     std::vector<int> removeTargetScriptList);
        ~ScriptCustom() = default;
    };

    //* ------------------- MOVE CONSTANTLY ------------------ *//

    class ScriptMoveConstantly
    {
    private:
        glm::vec2 _movement;
        std::vector<int> _targetEntityList;
        int _targetPage;

    public:
        int getTargetPageId();
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

    //* -------------------- SCRIPT UNION -------------------- *//

    union ScriptUnion
    {
        ScriptCustom scriptCustom;
        ScriptMoveConstantly scriptMoveConstantly;
        char space[1000];
        ScriptUnion(const ScriptUnion &other);
        ScriptUnion &operator=(const ScriptUnion &other);
        ScriptUnion();
        ScriptUnion(ScriptCustom scriptCustom);
        ScriptUnion(ScriptMoveConstantly scriptMoveConstantly);
        ~ScriptUnion();
    };

    //* -------------------- ANCHOR SCRIPT ------------------- *//

    class Script
    {
    private:
        int _scriptId;
        ScriptType _scriptType;
        std::string _scriptName;
        ScriptUnion _script;
        // for now, every thing only run once
        int life;

    public:
        std::string getScriptTypeString();
        int getScriptId();
        void setScriptId(int scripId);
        ScriptType getScriptType();
        void setScriptType(ScriptType scriptType);
        std::string getScriptName();
        void setScriptName(std::string scriptname);
        ScriptUnion getScript();
        void setScript(ScriptUnion script);

        /// \brief Parse function
        ///
        /// \param root
        /// \return Script
        static Script parse(nlohmann::json root);
        static ScriptType getScriptTypeFromString(std::string);

        /// \brief update all information of script
        /// *For use in editor, binding with update button in script editor.
        ///
        /// \param scriptType
        /// \param ... scriptId, scriptName, other variables. List of types:
        /// \param Custom std::vector<int> signal, std::vector<int>
        ///
        void updateScript(ScriptType scriptType, int id, std::string name...);

        /// \brief Construct a new Script object
        /// *For editor, used in Game::createScript()
        ///
        Script();

        /// \brief Construct a new Script object
        ///
        /// \param name
        Script(std::string name);

        Script(int scriptId, ScriptType scriptType, std::string scriptName, ScriptUnion script);
        ~Script();
    };

}