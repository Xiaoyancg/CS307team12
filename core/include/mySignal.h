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
        Key,
        Custom,
        CustomList
    };

    ////class BaseSignal
    ////{
    ////private:
    ////    /// \brief the ID
    ////    /// no signal ID for system defined signal
    ////    ///
    ////    int _signalId;
    ////    /// \brief the type of this signal
    ////    ///
    ////    SignalType _type;
    ////    std::string _signalName;
    ////public:
    ////    int getSignalId() { return _signalId; }
    ////    void setSignalId(int signalId) { _signalId = signalId; }
    ////    SignalType getSignalType() { return _type; }
    ////    void setSignalType(SignalType signalType) { _type = signalType; }
    ////    std::string getSignalName() { return _signalName; }
    ////    /// \brief Construct a new Base Signal object for system-defined signal
    ////    ///
    ////    BaseSignal() : _signalId(-1)
    ////    {
    ////    }
    ////    /// \brief Construct a new Base Signal object for parse function
    ////    ///
    ////    /// \param signalID
    ////    BaseSignal(int signalId) : _signalId(signalId) {}
    ////    ~BaseSignal() {}
    ////};

    //* --------------- ANCHOR TYPES OF SIGNALS -------------- *//

    /// \brief custom signal contains only the target script ID List
    /// when sending custom signal, it means the user want to evoke the target
    /// script. There will be no criteria to check in logic.
    /// Should be the default signal in editor.
    ///
    class CustomListSignal // : public BaseSignal
    {
    private:
        std::vector<int> _targetScriptList;

    public:
        std::vector<int> getTargetScriptList() { return _targetScriptList; }
        void setTargetScriptList(std::vector<int> targetScriptList)
        {
            _targetScriptList = targetScriptList;
        }
        CustomListSignal() : CustomListSignal(std::vector<int>()) {}
        CustomListSignal(std::vector<int> targetScriptList)
            : _targetScriptList(targetScriptList) {}
        ////CustomListSignal() : CustomListSignal(-1, std::vector<int>()) {}
        ////CustomListSignal( signalId, std::vector<int> targetScriptIDList)
        ////    : BaseSignal(signalId), _targetScriptIDList(targetScriptIDList) {}
        ~CustomListSignal() {}
    };

    /// \brief custom signal contains only the target script
    /// when sending custom signal, it means the user want to evoke the target
    /// script. There will be no criteria to check in logic.
    /// Should be the default signal in editor.
    ///
    class CustomSignal // : public BaseSignal
    {
    private:
        int _targetScript;

    public:
        int getTargetScript() { return _targetScript; }
        void setTargetScript(int targetScript)
        {
            _targetScript = targetScript;
        }
        static CustomSignal parse(nlohmann::json root);
        CustomSignal() : CustomSignal(-1) {}
        CustomSignal(int targetScript) : _targetScript(targetScript) {}
        ////    : BaseSignal(signalId), _targetScript(targetScript) {}
        ~CustomSignal() {}
    };

    /// \brief Key signals handles key event from SDL.
    /// Sends the key code and key type ( press / release ).
    /// System defined signal.
    ///
    class KeySignal
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
        SDL_Event event;
        /// \brief Construct a new Key Signal object with event
        ///
        /// \param event
        KeySignal(SDL_KeyboardEvent event)
            : _key(event.keysym.sym), _keyType(event.type) {}

        ~KeySignal() {}
    };

    //* ----------------- ANCHOR SIGNAL UNION ---------------- *//

    union SignalUnion
    {
        KeySignal keySignal;
        CustomSignal customSignal;
        CustomListSignal customListSignal;

        SignalUnion() {}
        SignalUnion(const SignalUnion &) = default;
        SignalUnion(KeySignal keySignal) : keySignal(keySignal) {}
        SignalUnion(CustomSignal customSignal) : customSignal(customSignal) {}
        SignalUnion(CustomListSignal customListSignal)
            : customListSignal(customListSignal) {}

        ~SignalUnion() {}
    };

    /// \brief the signal union contains all kind of signal and id and type
    ///
    class Signal
    {
    private:
        int _signalId;
        SignalType _signalType;
        //SignalUnion signal;
        SignalUnion _signal;

    public:
        static Signal parse(nlohmann::json root);
        Signal() : Signal(-1, SignalType::Custom, SignalUnion(CustomSignal())) {}
        Signal(int signalId, SignalType signalType, SignalUnion signal)
            : _signalId(signalId), _signalType(signalType), _signal(signal) {}
        ~Signal() {}
    };

}