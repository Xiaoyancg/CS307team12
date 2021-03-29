#pragma once
#include <map>
#include "Signal.h"
#include "Script.h"

namespace Core
{
    // forwar declaration
    enum class SignalType;
    enum class ScriptType;
    class Action;
    extern std::vector<Logic *> *gkeyLogicList;
    extern std::vector<Logic *> *gmouseLogicList;
    extern std::vector<Logic *> *gtimerLogicList;
    extern std::vector<Logic *> *gdirectLogicList;
    extern std::vector<Logic *> *gscriptList;

    // *classes list
    class KeyLogic;

    //* ----------------------- DIFFERENT LOGIC ---------------------- *//

    class KeyLogic
    {
    private:
        // based on SDL
        // key code
        SDL_KeyCode mkey;
        // press or release
        Uint32 mtype;

    public:
        void setKey(SDL_KeyCode key) { mkey = key; }
        SDL_KeyCode getKey() { return mkey; }
        void setType(Uint32 type) { mtype = type; }
        Uint32 getType() { return mtype; }

        // check for KeyLogic
        // singal is KeySignal
        bool check(Core::KeySignal signal);
        KeyLogic() {}
        ~KeyLogic() {}
    };

    typedef std::variant<KeyLogic> LogicVariant;
    //    typedef std::variant<LogicVariant, Action>;

    //* ------------------------- LOGIC ENTRY ------------------------ *//

    // should be used in the logic list waiting for signals
    // e.g, for each (signal in signalList) {
    //          switch (signal.type) {
    //              case key:
    //                  for each (logic in inputLogicList){
    //                      logic.check(signal);
    //                  }
    //                  break;
    //          }
    //      }
    //

    // we need Logic because the signal may need to pass value to mscript
    // also pair can't provite mscript type
    // And logic can be write to and load from json as object

    /// Use as pointer
    /// @param mlogic: instance of a specific logic
    /// @param mscript: a pointer to a script

    class Logic
    {
    private:
        SignalType msignalType;
        ScriptType mscriptType;
        LogicVariant mlogic;
        ScriptVariant mscript;
        // name starts with m for member

        bool mpause = true;

    public:
        void setSignalType(SignalType signalType) { msignalType = signalType; }
        SignalType getSignalType() { return msignalType; }
        void setScriptType(ScriptType actionType) { mscriptType = actionType; }
        ScriptType getScriptType() { return mscriptType; }
        void setLogic(LogicVariant logic) { mlogic = logic; }
        void setScript();

        ///
        /// @brief Check if signal meet the criteria and add this to ready logic
        /// ist if it meets
        ///
        /// @param signal
        ///
        void check(SignalVariant &signal);

        ///
        /// @brief Construct a new Logic object
        ///
        /// @param signalType
        /// @param actionType
        /// @param script
        ///
        Logic(SignalType signalType,
              ScriptType actionType,
              ScriptVariant script)
            : msignalType(signalType),
              mscriptType(actionType),
              mscript(script) {}

        ~Logic() {}
    };

}