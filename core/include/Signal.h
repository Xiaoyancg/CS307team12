#pragma once
#include <variant>
#include <SDL.h>

namespace Core
{

    // Event(signal and response) types
    enum class EventType
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
    typedef std::variant<KeySignal> SignalVariant;

    //* ------------------------------ entry signal ------------------------------ */
    // *entry class
    class Signal
    {
    private:
        union
        {
            KeySignal mks;
        };
        EventType mtype;

    public:
        EventType getType() { return mtype; }
        void setType(EventType type) { mtype = type; }

        // get the signal in specific Signaltype
        // when init, use this to get reference and then use the constructor of the specific signal to construct it.
        //TODO: need to test and add example here
        // When use in checking condition in logic, use this to get the signal and call its get function
        SignalVariant &getSignal();
        Signal(EventType type) : mtype(type) {}
        ~Signal() {}
    };
}