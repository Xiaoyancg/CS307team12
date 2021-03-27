#include "Response.h"
namespace Core
{
    void Response::inti()
    {
        switch (this->mtype)
        {
        case SignalType::key:
            this->mkr = KeyResponse();
            break;

        default:
            break;
        }
    }
    bool KeyResponse::check(Core::Signal signal)
    {

        return (signal.getType());
    }
}