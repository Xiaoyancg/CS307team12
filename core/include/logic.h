#pragma once
#include "mySignal.h"
#include "Script.h"

namespace Core
{

    //* ----------------------- CUSTOM ----------------------- *//

    class LogicCustom
    {
    private:
    public:
        LogicCustom();
        ~LogicCustom();
    };

    //* ------------------------- KEY ------------------------ *//

    class LogicKey
    {
    private:
        SDL_Keycode _key;

        /// \brief SDL_PRESSED or SDL_RELEASED
        ///
        Uint32 _keyType;

    public:
        bool check(SignalKey keySignal);
        LogicKey();
        LogicKey(SDL_Keycode key, Uint32 keyType);
        ~LogicKey();
    };

    //* --------------------- LOGIC UNION -------------------- *//
    union LogicUnion
    {
        LogicCustom customLogic;
        LogicKey keyLogic;
        LogicUnion &operator=(const LogicUnion &other);
        LogicUnion(const LogicUnion &other);
        LogicUnion();
        LogicUnion(LogicCustom customLogic);
        LogicUnion(LogicKey LogicKey);
        ~LogicUnion();
    };
    //* -------------------- ANCHOR LOGIC -------------------- *//

    class Logic
    {
    private:
        int _logicId;
        SignalType _signalType;
        ScriptType _scriptType;
        std::string _logicName;
        std::vector<int> _targetScriptList;
        LogicUnion _logic;

    public:
        void setLogicId(int logicId);
        int getLogicId();
        void setSignalType(SignalType signalType);
        SignalType getSignalType();
        void setScriptType(ScriptType scriptType);
        ScriptType getScriptType();
        void setLogicName(std::string logicName);
        std::string getLogicName();
        void setTargetScriptList(std::vector<int> targetScriptList);
        std::vector<int> getTargetScriptList();
        void setLogic(LogicUnion logic);
        LogicUnion getLogic();

        /// \brief update all information of a logic
        /// *For editor, bindint to the button "update" in logic editor
        ///
        /// \param signalType
        /// \param ...
        void updateLogic(SignalType signalType, ...);

        /// \brief Parse function
        ///
        /// \param root
        /// \return Logic
        static Logic parse(nlohmann::json root);

        /// \brief Construct a new default Logic object and add it to list
        /// *For editor, used in Game::createLogic().
        /// id -1, SignalType::Custom, ScriptType::Custom, "", [],
        /// empty LogicCustom
        ///
        Logic();

        Logic(int logicId, SignalType signalType, ScriptType scriptType,
              std::string logicName, std::vector<int> targetScriptList,
              LogicUnion logic);
        ~Logic();
    };

}