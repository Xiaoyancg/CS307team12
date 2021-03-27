#pragma once
#include "Action.h"
namespace Core
{
    // forwar declaration
    enum class EventType;
    enum class ActionType;
    class Signal;
    class Response;
    class Action;
    class Logic;
    // *logic class connecting response and action
    // there are two usage of construct Logic:
    //-------1 is in editor, 2 is when parse.
    // In both circumstance, user knows the type of Logic,
    // type checking can be loose
    class Logic
    {
    private:
        Action maction;
        Response mresponse;
        bool mready;

    public:
        void check();
        Logic(/* args */);
        ~Logic();
    };
}