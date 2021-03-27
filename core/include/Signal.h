#pragma once
#include <variant>
#include <SDL.h>

namespace Core
{

    // Event(signal and response) types
    enum class SignalType
    {
        key, // key event use SDL_KeyboardEvent
        mouse,
        time
    };

    // *classes
    // NOTE can't use forward declaration because we forward declaration can't tell compiler how big the type is.
    class KeySignal;

    //* ---------------------------- different signals --------------------------- */
    // *base signal class
    // the base signal for
    class SignalBase
    {
    public:
        bool permant = false;
    };

    // key signal
    class KeySignal : Core::SignalBase
    {
    private:
        SDL_KeyboardEvent mevent;

    public:
        KeySignal(SDL_KeyboardEvent event) : mevent(event) {}
        ~KeySignal() {}

        Uint32 getType() { return mevent.type; }
        SDL_Keycode getKey() { return mevent.keysym.sym; }
    };

    // *the signal variant type to mix all signal types
    typedef std::variant<KeySignal> SignalEvent;

    //* ------------------------------ entry signal ------------------------------ */
    // *entry class
    class Signal
    {
    private:
        union
        {
            KeySignal mks;
        };
        SignalType mtype;

    public:
        SignalType getType() { return mtype; }
        void setType(SignalType type) { mtype = type; }
        // get the signal in specific Signaltype
        SignalEvent &getSignal();
        Signal(SignalType type) : mtype(type) {}
        ~Signal() {}
    };
}