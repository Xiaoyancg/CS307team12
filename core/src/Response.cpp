#include "Response.h"
namespace Core
{
    void Response::inti()
    {
        switch (this->mtype)
        {
        case ResponseType::key:
            this->kr = KeyResponse();
            break;

        default:
            break;
        }
    }
    bool KeyResponse::keyResponse(SDL_KeyboardEvent event)
    {
        return event.keysym.sym == mkey && event.type == mtype;
    }
}