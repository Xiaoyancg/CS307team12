#pragma once
#include <variant>
#include <utility>
#include <vector>
#include <SDL.h>
namespace Core
{

    // Event(signal and response) types
    enum class SignalType
    {
        Key,   // key event use SDL_KeyboardEvent
        Mouse, // mouse event use SDL TODO
        Timer
    };

    // *classes
    class KeySignal;

    //* ------------------ DIFFERENT SIGNALS ----------------- *//

    //* base signal class
    class SignalBase
    {
    private:
        // signal type
        SignalType mtype;
        // true if the signal should stay in checking list
        // default is false
        bool mpermanent = false;

    public:
        // type, signaltype
        SignalType getType() { return this->mtype; }
        int setType(SignalType type) { this->mtype = type; }
        // permanent
        bool isPermanent() { return this->mpermanent; }
        int setPermanent(bool permanent) { this->mpermanent = permanent; }
        SignalBase(SignalType type) : mtype(type) {}
        ~SignalBase() {}
    };

    //* key signal
    class KeySignal : Core::SignalBase
    {
    private:
        // keyboard Event
        SDL_KeyboardEvent mevent;

    public:
        KeySignal(SDL_KeyboardEvent event) : mevent(event) {}
        ~KeySignal() {}

        // get SDL key event type ( press or release )
        Uint32 getKeyType() { return mevent.type; }
        SDL_Keycode getKey() { return mevent.keysym.sym; }

        KeySignal(SDL_KeyboardEvent event)
            : SignalBase(SignalType::Key), mevent(event) {}
        ~KeySignal() {}
    };

    // *the signal variant type containing all signal classes
    typedef std::variant<
        KeySignal>
        SignalVariant;

}