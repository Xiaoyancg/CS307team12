#pragma once
#include <map>
#include "Signal.h"
#include "Action.h"

namespace Core
{
    // forwar declaration
    enum class SignalType;
    enum class ActionType;
    class Signal;
    class Action;

    // *classes list
    class KeyLogic;

    //* --------------------------- different Logic -------------------------- */
    class KeyLogic
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

        // check for KeyLogic
        // singal is KeySignal
        bool check(Core::Signal signal);
        KeyLogic() {}
        ~KeyLogic() {}
    };

    typedef std::variant<KeyLogic> LogicVariant;
    //    typedef std::variant<LogicVariant, Action>;
    //* -----------------------------Logic Entry  ----------------------------- */

    // should be used in the logic list waiting for signals
    // e.g, for each (signal in signalList) {
    //          switch (signal.type) {
    //              case key:
    //                  for each (logic in inputLogicList){
    //                      logic.check(signal);
    //                  }
    //                  break;
    //          }
    //      }
    //

    // we need Logic because the signal may need to pass value to action
    // also pair can't provite action type
    // And logic can be write to and load from json as object
    class Logic
    {
    private:
        SignalType msignalType;
        ActionType mactionType;
        ActionVariant action;

        // name starts with m for member
        union
        {
            Core::KeyLogic mkr;
            // add more here
        };
        //bool mready = false;

    public:
        SignalType getSignalType() { return msignalType; }
        void setSignalType(SignalType signalType) { msignalType = signalType; }
        ActionType getActionType() { return mactionType; }
        void setActionType(ActionType actionType) { mactionType = actionType; }
        bool check(SignalVariant signal);
        void setAction();
        Logic(SignalType signalType, ActionType actionType, ActionVariant action)
            : msignalType(signalType), mactionType(actionType) {}
        ~Logic() {}
    };

    std::vector<Logic> *keyLogicList;
    std::vector<Logic> *mouseLogicList;
    std::vector<Logic> *timerLogicList;
    std::vector<Logic> *directLogicList;

}