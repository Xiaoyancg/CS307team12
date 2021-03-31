#pragma once
#include <variant>
#include <utility>
#include <vector>
#include <SDL.h>
namespace Core
{

    /// anchor SingalType
    /// \brief Event(signal and logic) types
    /// \note there are predefined signal and user defined signal
    /// we can't just use user defined signal.
    /// User defined signal is just an id of logic
    /// if only use user defined signal, we have to parse every sdl event in
    /// runtime instead of before compile.
    /// same for logic, there are user defined logic which only compares id from
    /// signal, and there are predefined logic to check the input signal.
    enum class SignalType
    {
        Key,   // key event use SDL_KeyboardEvent
        Mouse, // mouse event use SDL TODO
        Timer
    };

    // anchor - signal base class
    //* --------------------- SIGNAL BASE -------------------- *//

    /// \brief Contains the information needs to be checked in corresponding
    /// logics.
    /// \note Are user defined and will be stored in Json. use reference
    /// mostly. Managed in an unordered_map in SignalManager
    class SignalBase
    {
    private:
        // signal type
        SignalType mtype;
        std::string msignalName;
        // true if the signal should stay in checking list
        // default is false
        bool mpermanent = false;

    public:
        // type, signaltype
        SignalType getType() { return this->mtype; }
        int setType(SignalType type) { this->mtype = type; }
        // name
        void setSignalName(std::string &signalName_ref)
        {
            msignalName = signalName_ref;
        }
        std::string getSignalName() { return msignalName; }
        // permanent
        bool isPermanent() { return this->mpermanent; }
        int setPermanent(bool permanent) { this->mpermanent = permanent; }
        SignalBase(std::string &signalName_ref, SignalType type)
            : msignalName(signalName_ref), mtype(type) {}
        ~SignalBase() {}
    };

    //* ------------------ DIFFERENT SIGNALS ----------------- *//

    // anchor - key signal
    /// \brief a SDL_KeyboardEvent signal
    class KeySignal : public SignalBase
    {
    private:
        /// \brief SDL keyboard Event
        /// \note use instance because the event may disappear after sdl event
        /// loop
        SDL_KeyboardEvent mevent;

    public:
        SDL_Keycode getKey() { return mevent.keysym.sym; }
        // get SDL key event type ( press or release )
        Uint32 getKeyType() { return mevent.type; }

        KeySignal(std::string &signalName_ref, SDL_KeyboardEvent event)
            : SignalBase(signalName_ref, SignalType::Key), mevent(event) {}
        ~KeySignal() {}
    };

    // anchor - signal variant type
    // containing all signal classes
    typedef std::variant<
        KeySignal>
        Signal;

}