#pragma once
#include <map>
#include "Signal.h"
#include "Script.h"

namespace Core
{
    // forward declaration
    enum class SignalType;
    enum class ScriptType;

    // *classes list
    class KeyLogic;

    // anchor - Logic Class Base

    /// \brief the base logic class that contains the criteria to meet to evoke
    /// a certain script
    ///
    /// \note A logic can be written to and load from json as object.
    /// will be stored in a unordered_map in LogicManager since it exists both
    /// conceptually and physically. Logic will have no clue of the game state.
    /// the only information it has are the signal and criteria. sending script
    /// to script list is done by game to limit the DI in minimum.
    class LogicBase
    {
    private:
        // name starts with m for member
        SignalType msignalType;
        ScriptType mscriptType;

        /// \brief Each logic will have a script pointer to get evoked
        ///
        /// \note The reason why don't use id and an unordered_map for script is
        /// that the script usually contains customized variables.
        /// Also, normally the functions in class won't be compiled for each
        /// instance. So we can have instane with functions and data
        /// without to concern the redundant memory.
        /// use reference. As the game stays, the instance will be stored in the
        /// unordered_map.
        ScriptVariant mscript_ref;

        // true if the logic is paused in the list and shouldn't be check
        // didn't implemented yet 03/30
        bool mpause = true;

        /// \brief the times it will stay in the logic checking list.
        /// \0         should be deleted
        /// \positive    minus one and stay
        /// \negative    stay in list until changed by scripts
        int lifespan;

    public:
        // signal type
        void setSignalType(SignalType signalType) { msignalType = signalType; }
        SignalType getSignalType() { return msignalType; }
        // script type
        void setScriptType(ScriptType actionType) { mscriptType = actionType; }
        ScriptType getScriptType() { return mscriptType; }
        // script
        void setScriptPtr(ScriptVariant script_ref)
        {
            mscript_ref = script_ref;
        }
        ScriptVariant getScriptPtr() { return mscript_ref; }

        /// \brief Check if signal meet the criteria
        ///
        /// \param signal passed by reference
        /// \return true if signal meets criteria
        virtual bool check(SignalVariant &signal_ref) = 0;

        /// \brief Construct a new Logic Base object
        ///
        /// \param signalType
        /// \param scriptType
        /// \param script_ref the target script to evoke if the specific criteria
        /// meets
        LogicBase(SignalType signalType,
                  ScriptType scriptType,
                  ScriptVariant &script_ref)
            : msignalType(signalType),
              mscriptType(scriptType),
              mscript_ref(script_ref) {}
        ~LogicBase() {}
    };

    //* ------------------ DIFFERENT LOGICS ------------------ *//

    // anchor - Key logic
    class KeyLogic : public LogicBase
    {
    private:
        // based on SDL key event
        // key code
        SDL_KeyCode mkey;
        // press or release key type
        Uint32 mkeyType;

    public:
        // key code
        void setKey(SDL_KeyCode key) { mkey = key; }
        SDL_KeyCode getKey() { return mkey; }
        // key type (press/release)
        void setKeyType(Uint32 type) { mkeyType = type; }
        Uint32 getKeyType() { return mkeyType; }

        /// \brief check if the key code and key type is met
        ///
        /// \param signal reference, type is KeySignal
        /// \return true if the key is the right key and in the correct type
        bool check(SignalVariant &signal_ref);

        /// \brief Construct a new Key Logic object. Set key and keytype before
        /// check, detain in comments at virtual function
        ///
        /// \param signalType
        /// \param scriptType
        /// \param script_ref
        KeyLogic(SignalType signalType,
                 ScriptType scriptType,
                 ScriptVariant &script_ref)
            : LogicBase(signalType, scriptType, script_ref) {}
        ~KeyLogic() {}
    };

    // anchor - logic variant
    // containing all specific logic class type
    typedef std::variant<KeyLogic> LogicVariant;
}