#include <vector>
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

    //* ----------------- ANCHOR SCRIPT TYPES ---------------- *//

    //* ----------------------- CUSTOM ---------------------- *//

    class ScriptCustom
    {
    private:
        std::vector<int> _targetSignalList;
        std::vector<int> _targetLogicList;
        std::vector<int> _targetScriptList;
        /// \brief true to send, false to remove
        ///
        bool _action;

    public:
        std::vector<int> getTargetSignalList();
        std::vector<int> getTargetLogicList();
        std::vector<int> getTargetScriptList();
        bool getAction();
        void setTargetSignalList(std::vector<int> targetSignalList);
        void setTargetLogicList(std::vector<int> targetLogicList);
        void setTargetScriptList(std::vector<int> targetScriptList);
        void setAction(bool action);

        ScriptCustom();

        ScriptCustom(std::vector<int> targetSignalList,
                     std::vector<int> targetLogicList,
                     std::vector<int> targetScriptList, bool action);
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

    //* -------------------- SCRIPT UNION -------------------- *//

    union ScriptUnion
    {
        ScriptCustom scriptCustom;
        ScriptMoveConstantly scriptMoveConstantly;
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

    public:
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

        /// \brief update all information of script
        /// *For use in editor, binding with update button in script editor.
        ///
        /// \param scriptType
        /// \param ... scriptId, scriptName, other variables. List of types:
        /// \param Custom std::vector<int> signal, std::vector<int>
        ///
        void updateScript(ScriptType scriptType, ...);

        /// \brief Construct a new Script object
        /// *For editor, used in Game::createScript()
        ///
        Script();

        Script(int scriptId, ScriptType scriptType, std::string scriptName, ScriptUnion script);
        ~Script();
    };

}