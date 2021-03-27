#pragma once
#include "signal.h"

namespace Core
{
    // forwar declaration
    enum class SignalType;
    class Signal;

    // *classes list
    class KeyResponse;

    //* --------------------------- different response -------------------------- */
    class KeyResponse
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

        // check for KeyResponse
        // singal is KeySignal
        bool check(Core::Signal signal);
        KeyResponse() {}
        ~KeyResponse() {}
    };

    typedef std::variant<KeyResponse> ResponseEvent;
    //* ----------------------------- Entry Response ----------------------------- */
    class Response
    {
    private:
        SignalType mtype;

        // name starts with m for member
        union
        {
            Core::KeyResponse mkr;
            // add more here
        };
        bool mready = false;

    public:
        SignalType getType() { return mtype; }
        void setType(SignalType type) { mtype = type; }
        bool isReady() { return mready; }
        void setReady(bool ready) { mready = ready; }
        // must call after settype

        bool response(Signal signal);
        Response() {}
        ~Response() {}
    };

}