#pragma once
#include <SDL.h>
namespace Core
{
    enum class SignalType
    {
        Key
    };

    class BaseSignal
    {
    private:
        /// \brief the ID
        ///
        int _signalID;

        /// \brief the type of this signal
        ///
        SignalType _type;

    public:
        int getSignalID() { return _signalID; }
        void setSignalID(int signalID) { _signalID = signalID; }
        SignalType getSignalType() { return _type; }
        void setSignalType(SignalType type) { _type = type; }
        BaseSignal(/* args */) {}
        ~BaseSignal() {}
    };

    //* --------------- ANCHOR TYPES OF SIGNALS -------------- *//

    /// \brief custom signal contains only the target script
    /// when sending custom signal, it means the user want to evoke the target
    /// script. There will be no criteria to check in logic.
    /// Should be the default signal in editor.
    ///
    class CustomSignal : public BaseSignal
    {
    private:
        int _targetScriptID;

    public:
        int getTargetScriptID() { return _targetScriptID; }
        void setTargetScriptID(int targetScriptID)
        {
            _targetScriptID = targetScriptID;
        }
        CustomSignal() {}
        CustomSignal(int targetScriptID) : _targetScriptID(targetScriptID) {}
        ~CustomSignal() {}
    };

    /// \brief Key signals handles key event from SDL
    /// it send the key code and key type ( press / release )
    ///
    class KeySignal : public BaseSignal
    {
    private:
        /// \brief SDL key code
        ///
        SDL_Keycode _key;

        /// \brief key type ( SDL_PRESSED or SDL_RELEASED )
        ///
        Uint32 _keyType;

    public:
        KeySignal() {}

        /// \brief Construct a new Key Signal object with event
        ///
        /// \param event
        KeySignal(SDL_KeyboardEvent event)
            : _key(event.keysym.sym), _keyType(event.type) {}

        ~KeySignal() {}
    };

    //* ----------------- ANCHOR SIGNAL UNION ---------------- *//

    /// \brief the signal union contains all kind of signal
    ///
    union Signal
    {
        KeySignal keySignal;
        CustomSignal customSignal;
    };
}