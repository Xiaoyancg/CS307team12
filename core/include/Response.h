#pragma once
#include "Game.h"
#include <any>

namespace Core
{
    // classes
    class KeyResponse;

    enum class ResponseType
    {
        key,
        mouse,
        time
    };

    // the class for outside
    class Response
    {
    private:
        ResponseType mtype;
        union
        {
            KeyResponse kr;
            // add more here
        };

    public:
        ResponseType getType() { return mtype; }
        void setType(ResponseType type) { mtype = type; }
        // must call after settype
        void inti();
        bool response(std::any signal);
        Response() {}
        ~Response() {}
    };

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
        bool keyResponse(SDL_KeyboardEvent event);
        KeyResponse() {}
        ~KeyResponse() {}
    };

}