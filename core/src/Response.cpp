#include "Response.h"
namespace Core
{
    bool KeyResponse::keyResponse(SDL_KeyboardEvent event)
    {
        return event.keysym.sym == mkey && event.type == mtype;
    }
}