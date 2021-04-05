#pragma once
#include <SDL.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
namespace Core
{
    union SignalUnion;

    enum class SignalType
    {
        Custom = 0,
        Key = 1,
    };

    //* --------------- ANCHOR TYPES OF SIGNALS -------------- *//

    /// \brief custom signal contains only the target script ID List
    /// when sending custom signal, it means the user want to evoke the target
    /// script. There will be no criteria to check in logic.
    /// Should be the default signal in editor.
    ///
    class SignalCustom // : public BaseSignal
    {
    private:
        std::vector<int> _targetScriptList;

    public:
        std::vector<int> getTargetScriptList() { return _targetScriptList; }
        void setTargetScriptList(std::vector<int> targetScriptList)
        {
            _targetScriptList = targetScriptList;
        }
        SignalCustom() : SignalCustom(std::vector<int>()) {}
        SignalCustom(std::vector<int> targetScriptList)
            : _targetScriptList(targetScriptList) {}
        ~SignalCustom() {}
    };

    /// \brief Key signals handles key event from SDL.
    /// Sends the key code and key type ( press / release ).
    /// System defined signal.
    ///
    class SignalKey
    {
    private:
        /// \brief SDL key code
        ///
        SDL_Keycode _key;

        /// \brief key type ( SDL_PRESSED or SDL_RELEASED )
        ///
        Uint32 _keyType;

    public:
        SignalKey() {}
        SDL_Event event;
        /// \brief Construct a new Key Signal object with event
        ///
        /// \param event
        SignalKey(SDL_KeyboardEvent event)
            : _key(event.keysym.sym), _keyType(event.type) {}

        ~SignalKey() {}
    };

    //* ----------------- ANCHOR SIGNAL UNION ---------------- *//

    union SignalUnion
    {
        SignalKey keySignal;
        SignalCustom customSignal;

        SignalUnion() {}
        SignalUnion(const SignalUnion &) = default;
        SignalUnion(SignalKey keySignal) : keySignal(keySignal) {}
        SignalUnion(SignalCustom customSignal) : customSignal(customSignal) {}

        ~SignalUnion() {}
    };

    /// \brief the signal union contains all kind of signal and id and type
    ///
    class Signal
    {
    private:
        int _signalId;
        SignalType _signalType;
        SignalUnion _signal;

    public:
        static Signal parse(nlohmann::json root);

        /// \brief Construct a new Signal object by default.
        /// Id is -1, type is custom.
        ///
        Signal() : Signal(-1, SignalType::Custom, SignalUnion(SignalCustom())) {}

        /// \brief Construct a new Signal object
        ///
        /// \param signalId
        /// \param signalType
        /// \param signal
        Signal(int signalId, SignalType signalType, SignalUnion signal)
            : _signalId(signalId), _signalType(signalType), _signal(signal) {}

        ~Signal() {}
    };

}