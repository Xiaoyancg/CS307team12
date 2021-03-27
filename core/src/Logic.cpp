#include "Logic.h"
namespace Core
{
    void Logic::init()
    {
        switch (this->mtype)
        {
        case EventType::key:
            this->mkr = KeyLogic();
            break;

        default:
            break;
        }
    }
    bool KeyLogic::check(Core::Signal signal)
    {

        return (signal.getType());
    }
}