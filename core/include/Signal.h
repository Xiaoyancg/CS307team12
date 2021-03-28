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
        key, // key event use SDL_KeyboardEvent
        mouse,
        timer
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
    typedef std::pair<SignalVariant, SignalType> Signal;

    // the signalList that game loop checks
    std::vector<Signal> signalList;

    //  // ------------------------------ entry signal ------------------------------ */
    // // *entry class
    ////class Signal
    ////{
    ////private:
    ////union
    ////{
    ////KeySignal mks;
    ////};
    ////SignalType mtype;

    ////public:
    ////SignalType getType() { return mtype; }
    ////void setType(SignalType type) { mtype = type; }

    ////// get the signal in specific Signaltype
    ////// when init, use this to get reference and then use the constructor of the specific signal to construct it.
    //// // : need to test and add example here
    ////// When use in checking condition in logic, use this to get the signal and call its get function
    ////SignalVariant &getSignal();
    ////Signal(SignalType type) : mtype(type) {}
    ////~Signal() {}
    ////};
}