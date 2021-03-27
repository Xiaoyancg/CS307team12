#pragma once
#include "Action.h"
#include "Signal.h"
namespace Core
{

    // logic class contains all the information needed to achieve the designed result
    // support functions: {
    //      set current page
    // }
    enum class SignalType
    {
        key,
        mouse,
        time
    };
    enum class ActionType
    {
        moveEntity
    };

    // there are two usage of construct Logic:
    //-------1 is in editor, 2 is when parse.
    // In both circumstance, user knows the type of Logic,
    // type checking can be loose
    class Logic
    {
    private:
        // this logic type
        SignalType msignalT;
        ActionType mactionT;
        Action maction;

    public:
        SignalType getSignalType() { return msignalT; }
        ActionType getActionType() { return mactionT; }
        void setSignalType(SignalType signaltype) { msignalT = signaltype; }
        void setActionType(ActionType actiontype) { mactionT = actiontype; }
        //void evoke();
        Logic(/* args */);
        ~Logic();
    };
}