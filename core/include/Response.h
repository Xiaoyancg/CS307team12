#pragma once
#include "Game.h"

namespace Core
{
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

    class Response : KeyResponse
    {
    private:
        /* data */
    public:
        Response() {}
        ~Response() {}
    };

}