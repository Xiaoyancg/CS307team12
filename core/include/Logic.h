#pragma once
#include "Action.h"
#include "Response.h"
namespace Core
{

    // logic class contains all the information needed to achieve the designed result
    // support functions: {
    //      set current page
    // }

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
        ActionType mactionT;
        Action maction;

    public:
        ActionType getActionType() { return mactionT; }
        void setActionType(ActionType actiontype) { mactionT = actiontype; }
        //void evoke();
        Logic(/* args */);
        ~Logic();
    };
}